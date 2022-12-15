#include "PscDecompiler.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <iterator>


#include "Node/Nodes.hpp"
#include "Node/WithNode.hpp"
#include "Node/NodeComparer.hpp"

#include "PscCodeGenerator.hpp"

static inline
bool isTempVar(const Pex::StringTable::Index& var)
{
    auto& name = var.asString();
    return (name.length() > 6 && name.substr(0, 6) == "::temp" && name.substr(name.length() - 4, 4) != "_var") || _stricmp(name.c_str(), "::nonevar") == 0;
}
static inline
bool isMangledVar(const Pex::StringTable::Index& var)
{
    auto& name = var.asString();
    return name.length() > 12 && name.substr(0, 10) == "::mangled_";
}

static inline
std::string getVarName(const Pex::StringTable::Index& var)
{
    auto& name = var.asString();
    if (name.length() > 6 && name.substr(0, 2) == "::" && name.substr(name.length() - 4, 4) == "_var")
    {
        return name.substr(2, name.length() - 6);
    }
    else if (name.length() > 12 && name.substr(0, 10) == "::mangled_")
    {
        auto index = name.rfind('_');
        return name.substr(10, index - 10);
    }

    return name;
}

/**
 * @brief Constructor.
 * The constructor associate the function and object to the decompiler.
 *
 * @param function Function to decompile.
 * @param object Object containing the function.
 * @param commentAsm True to output assembly instruction comments.
 */
Decompiler::PscDecompiler::PscDecompiler(const Pex::Function &function, const Pex::Object &object, bool commentAsm = false) :
    m_Function(function),
    m_Object(object),
    m_CommentAsm(commentAsm)
{
    if (m_Function.getInstructions().size() == 0)
    {
        push_back("; Empty function");
    }
    else
    {
        m_ReturnNone = (m_Function.getReturnTypeName() == m_Object.getName().getTable()->findIdentifier("NONE"));
        m_TempTable.push_back("true");
        m_TempTable.push_back("false");

        m_TempTable.push_back("find");
        m_TempTable.push_back("findstruct");
        m_TempTable.push_back("rfind");
        m_TempTable.push_back("rfindstruct");
        m_TempTable.push_back("add");
        m_TempTable.push_back("insert");
        m_TempTable.push_back("removelast");
        m_TempTable.push_back("remove");
        m_TempTable.push_back("clear");
        m_TempTable.push_back("getallmatchingstructs");

        //findReplacedVars();
        findVarTypes();

        createFlowBlocks();

        rebuildExpressionsInBlocks();

        rebuildBooleanOperators(0, m_Function.getInstructions().size());


        Node::BasePtr programTree = rebuildControlFlow(0, m_Function.getInstructions().size());

        declareVariables(programTree);
        cleanUpTree(programTree);

        generateCode(programTree);

    }
}

/**
 * @brief Destructor
 */
Decompiler::PscDecompiler::~PscDecompiler()
{
    for (auto& bloc_kv : m_CodeBlocs)
    {
        auto& bloc = bloc_kv.second;
        delete bloc;
    }
}

/**
 * @brief Output a range of instruction as assembly comment.
 * @param level Indentation level.
 * @param begin First instruction to output.
 * @param end Last instruction to output.
 */

void Decompiler::PscDecompiler::decodeToAsm(std::uint8_t level, size_t begin, size_t end)
{
    if (m_CommentAsm)
    {
        auto& instructions = m_Function.getInstructions();

        if (begin >= instructions.size() || end >= instructions.size())
        {
            return;
        }

        for (auto ip = begin; ip <= end; ++ip)
        {
            auto& ins = instructions[ip];
            std::ostringstream stream;
            for (auto i = 0; i < level; ++i)
            {
                stream << ' ' << ' ';
            }
            stream << "; " << std::setw(3) << std::setfill('0') << ip << " : " << ins.getOpCodeName() << " ";
            switch(ins.getOpCode())
            {
            case Pex::OpCode::JMP:
            {
                assert(ins.getArgs().size() == 1);
                assert(ins.getArgs()[0].getType() == Pex::ValueType::Integer);

                auto target = ip + ins.getArgs()[0].getInteger();
                stream << std::setw(3) << std::setfill('0') << target;
            }
                break;
            case Pex::OpCode::JMPF:
            case Pex::OpCode::JMPT:
            {
                assert(ins.getArgs().size() == 2);
                assert(ins.getArgs()[1].getType() == Pex::ValueType::Integer);

                stream << ins.getArgs()[0].toString() << " ";
                auto target = ip + ins.getArgs()[1].getInteger();
                stream << std::setw(3) << std::setfill('0') << target;

            }
                break;
            default:
            {
                for (auto& arg : ins.getArgs())
                {
                    stream << arg.toString() << " ";
                }

                if (ins.hasVarArgs())
                {
                    for (auto& arg : ins.getVarArgs())
                    {
                        stream << arg.toString() << " ";
                    }
                }
            }
                break;
            }


            push_back(stream.str());
        }
    }
}

/**
 * @brief Finds the type of the variables.
 *
 * This function finds the type of the local variables and parameters accessible in the function's scope.
 * The types are stored in m_VarTypes.
 */
void Decompiler::PscDecompiler::findVarTypes()
{
    for (auto& var : m_Object.getVariables())
        m_VarTypes[var.getName().asIndex()] = var.getTypeName();
    for (auto& var : m_Function.getParams())
        m_VarTypes[var.getName().asIndex()] = var.getTypeName();
    for (auto& var : m_Function.getLocals())
        m_VarTypes[var.getName().asIndex()] = var.getTypeName();

    m_NoneVar = m_Object.getName().getTable()->findIdentifier("::nonevar");
}

/**
 * @brief Get the type of a variable.
 * @param var Name index of the variable.
 * @return The name index of the type.
 */
const Pex::StringTable::Index& Decompiler::PscDecompiler::typeOfVar(const Pex::StringTable::Index &var) const
{
    static const Pex::StringTable::Index UNDEFINED;
    auto it = m_VarTypes.find(var.asIndex());
    if (it == m_VarTypes.end())
    {
        return UNDEFINED;
    }
    else
    {
        assert(it->second.isValid());
        return it->second;
    }
}

/**
 * @brief Creates the code blocks graph.
 *
 * This function goes through the instruction list and creates the code block representation
 * to allow further flow analysis. Each block contains an uninterrupted sequence of instruction which will be
 * reconstructed as statements sequence. A blocks ends at a jump, conditional or not. The target of the jump
 * will mark the beginning of a new block.
 */
void Decompiler::PscDecompiler::createFlowBlocks()
{
    auto& instructions = m_Function.getInstructions();
    auto full = new PscCodeBlock(0, instructions.size() - 1);
    full->setNext(instructions.size());

    m_CodeBlocs[full->getBegin()] = full;
    m_CodeBlocs[instructions.size()] = new PscCodeBlock(instructions.size(), PscCodeBlock::END);

    auto ip = 0;
    for (auto& ins : instructions)
    {
        auto block = findBlockForInstruction(ip);
        switch(ins.getOpCode())
        {     
        case Pex::OpCode::JMP:
        {
            assert(ins.getArgs().size() == 1);
            assert(ins.getArgs()[0].getType() == Pex::ValueType::Integer);

            auto target = ip + ins.getArgs()[0].getInteger();

            // Unconditional jump
            // Split the block at the jump and set the next block to the target of the jump.
            if (m_CodeBlocs.find(ip+1) == m_CodeBlocs.end())
            {
                auto newBlock = m_CodeBlocs[block]->split(ip+1);
                m_CodeBlocs[newBlock->getBegin()] = newBlock;

            }
            m_CodeBlocs[block]->setNext(target);

            if (m_CodeBlocs.find(target) == m_CodeBlocs.end())
            {
                auto containingBlock = m_CodeBlocs[findBlockForInstruction(target)];
                auto targetBlock = containingBlock->split(target);
                m_CodeBlocs[targetBlock->getBegin()] = targetBlock;
            }


        }
            break;
        case Pex::OpCode::JMPF:
        case Pex::OpCode::JMPT:
        {
            assert(ins.getArgs().size() == 2);
            assert(ins.getArgs()[0].getType() == Pex::ValueType::Identifier || ins.getArgs()[0].getType() == Pex::ValueType::Bool || ins.getArgs()[0].getType() == Pex::ValueType::Integer);
            assert(ins.getArgs()[1].getType() == Pex::ValueType::Integer);

            // Conditional jump
            // The block is split at the jump.
            // The block condition is set to the condition of the jump,
            // The true and false block are set to the target of the jump, and the instruction following the jump.
            // The true or false order is decided from the kind of jump (jmpf/jmpt).
            auto target = ip + ins.getArgs()[1].getInteger();
            if (m_CodeBlocs.find(ip+1) == m_CodeBlocs.end())
            {
                auto newBlock = m_CodeBlocs[block]->split(ip+1);
                m_CodeBlocs[newBlock->getBegin()] = newBlock;
            }

            if (m_CodeBlocs.find(target) == m_CodeBlocs.end())
            {
                auto containingBlock = m_CodeBlocs[findBlockForInstruction(target)];
                auto targetBlock = containingBlock->split(target);
                m_CodeBlocs[targetBlock->getBegin()] = targetBlock;
            }

            Pex::StringTable::Index condition;
            if (ins.getArgs()[0].getType() == Pex::ValueType::Identifier)
            {
                condition = ins.getArgs()[0].getId();
            }
            else if (ins.getArgs()[0].getType() == Pex::ValueType::Bool)
            {
                if(ins.getArgs()[0].getBool())
                {
                    condition = m_TempTable.findIdentifier("true");
                }
                else
                {
                    condition = m_TempTable.findIdentifier("false");
                }
            }
            else {
                condition = m_TempTable.get(ins.getArgs()[0].getInteger());
            }

            if (ins.getOpCode() == Pex::OpCode::JMPF)
            {
                m_CodeBlocs[block]->setCondition(condition, ip + 1, target);
            }
            else
            {
                m_CodeBlocs[block]->setCondition(condition, target, ip + 1);
            }
        }
            break;
        default:
            break;
        }

        ++ip;
    }

    // Creates the nodes for the blocs.
    for (auto& bloc_kv : m_CodeBlocs)
    {
        createNodesForBlocks(bloc_kv.first);
    }
}

/**
 * @brief Create the statement nodes for the block.
 *
 * This function creates the statement nodes for each instruction of the blocks.
 * From each instruction, a set of nodes representing the corresponding Papyrus statement.
 *
 * @param block Indice of the block.
 */
void Decompiler::PscDecompiler::createNodesForBlocks(size_t block)
{
    auto code = m_CodeBlocs[block];
    auto& instructions = m_Function.getInstructions();
    auto scope = code->getScope();
    if (code->getBegin() < instructions.size())
    {
        for(auto ip = code->getBegin(); ip <= code->getEnd(); ++ip)
        {
            auto& ins = instructions[ip];

            auto& args = ins.getArgs();
            auto& varargs = ins.getVarArgs();

            Node::BasePtr node;
            switch(ins.getOpCode())
            {
                case Pex::OpCode::NOP:
                    //do nothing
                    break;
                case Pex::OpCode::IADD:
                case Pex::OpCode::FADD:
                case Pex::OpCode::STRCAT:
                {
                    node = std::make_shared<Node::BinaryOperator>(ip, 5, args[0].getId(), fromValue(ip, args[1]), "+", fromValue(ip, args[2]));
                    break;
                }
                case Pex::OpCode::ISUB:
                case Pex::OpCode::FSUB:
                {
                    node = std::make_shared<Node::BinaryOperator>(ip, 5, args[0].getId(), fromValue(ip, args[1]), "-", fromValue(ip, args[2]));
                    break;
                }
                case Pex::OpCode::IMUL:
                case Pex::OpCode::FMUL:
                {
                    node = std::make_shared<Node::BinaryOperator>(ip, 4, args[0].getId(), fromValue(ip, args[1]), "*", fromValue(ip, args[2]));
                    break;
                }
                case Pex::OpCode::IDIV:
                case Pex::OpCode::FDIV:
                {
                    node = std::make_shared<Node::BinaryOperator>(ip, 4, args[0].getId(), fromValue(ip, args[1]), "/", fromValue(ip, args[2]));
                    break;
                }
                case Pex::OpCode::IMOD:
                {
                    node = std::make_shared<Node::BinaryOperator>(ip, 4, args[0].getId(), fromValue(ip, args[1]), "%", fromValue(ip, args[2]));
                    break;
                }
                case Pex::OpCode::NOT:
                {
                    node = std::make_shared<Node::UnaryOperator>(ip, 3, args[0].getId(), "!", fromValue(ip, args[1]));
                    break;
                }
                case Pex::OpCode::INEG:
                case Pex::OpCode::FNEG:
                {
                    node = std::make_shared<Node::UnaryOperator>(ip, 3, args[0].getId(), "-", fromValue(ip, args[1]));
                    break;
                }
                case Pex::OpCode::ASSIGN:
                {
                    node = std::make_shared<Node::Copy>(ip, args[0].getId(), fromValue(ip, args[1]));
                    break;
                }
                case Pex::OpCode::CAST:
                {
                    if(args[1].getType() == Pex::ValueType::None)
                    {
                        node = std::make_shared<Node::Copy>(ip, args[0].getId(), fromValue(ip, args[1]));
                    }
                    else if (args[1].getType() != Pex::ValueType::Identifier || (typeOfVar(args[0].getId()) != typeOfVar(args[1].getId()) && args[1].getId() != m_NoneVar) )
                    {                    
                        node = std::make_shared<Node::Cast>(ip, args[0].getId(), fromValue(ip, args[1]), typeOfVar(args[0].getId()));
                    }
                    else // two variables of the same type, equivalent to an assign
                    {
                        node = std::make_shared<Node::Copy>(ip, args[0].getId(), fromValue(ip, args[1]));
                    }
                    break;
                }
                case Pex::OpCode::CMP_EQ:
                {
                    node = std::make_shared<Node::BinaryOperator>(ip, 5, args[0].getId(), fromValue(ip, args[1]), "==", fromValue(ip, args[2]));
                    break;
                }
                case Pex::OpCode::CMP_LT:
                {
                    node = std::make_shared<Node::BinaryOperator>(ip, 5, args[0].getId(), fromValue(ip, args[1]), "<", fromValue(ip, args[2]));
                    break;
                }
                case Pex::OpCode::CMP_LTE:
                {
                    node = std::make_shared<Node::BinaryOperator>(ip, 5, args[0].getId(), fromValue(ip, args[1]), "<=", fromValue(ip, args[2]));
                    break;
                }
                case Pex::OpCode::CMP_GT:
                {
                    node = std::make_shared<Node::BinaryOperator>(ip, 5, args[0].getId(), fromValue(ip, args[1]), ">", fromValue(ip, args[2]));
                    break;
                }
                case Pex::OpCode::CMP_GTE:
                {
                    node = std::make_shared<Node::BinaryOperator>(ip, 5, args[0].getId(), fromValue(ip, args[1]), ">=", fromValue(ip, args[2]));
                    break;
                }
                case Pex::OpCode::JMP:
                case Pex::OpCode::JMPT:
                case Pex::OpCode::JMPF:
                    //do nothing
                    break;
                case Pex::OpCode::CALLMETHOD:
                {
                    auto callNode = std::make_shared<Node::CallMethod>(ip, args[2].getId(), fromValue(ip, args[1]), args[0].getId());
                    auto argNode = callNode->getParameters();
                    for (auto varg : varargs)
                    {
                        *argNode << fromValue(ip, varg);
                    }                
                    node = callNode;
                    break;
                }
                case Pex::OpCode::CALLPARENT:
                {
                    auto callNode = std::make_shared<Node::CallMethod>(ip, args[1].getId(), std::make_shared<Node::IdentifierString>(ip, "Parent"), args[0].getId());
                    auto argNode = callNode->getParameters();
                    for (auto varg : varargs)
                    {
                        *argNode << fromValue(ip, varg);
                    }
                    node = callNode;
                    break;
                }
                case Pex::OpCode::CALLSTATIC:
                {
                    auto callNode = std::make_shared<Node::CallMethod>(ip, args[2].getId(), fromValue(ip, args[0]), args[1].getId());
                    auto argNode = callNode->getParameters();
                    for (auto varg : varargs)
                    {
                        *argNode << fromValue(ip, varg);
                    }
                    node = callNode;
                    break;
                }
                case Pex::OpCode::RETURN:
                {
                    if(m_ReturnNone)
                    {
                        node = std::make_shared<Node::Return>(ip, nullptr);
                    }
                    else
                    {
                        node = std::make_shared<Node::Return>(ip, fromValue(ip, args[0]));
                    }
                    break;
                }

                case Pex::OpCode::PROPGET:
                {
                    node = std::make_shared<Node::PropertyAccess>(ip, args[2].getId(), fromValue(ip, args[1]), args[0].getId());
                    break;
                }
                case Pex::OpCode::PROPSET:
                {
                    node = std::make_shared<Node::PropertyAccess>(ip, Pex::StringTable::Index(), fromValue(ip, args[1]), args[0].getId());
                    node = std::make_shared<Node::Assign>(ip, node, fromValue(ip, args[2]));
                    break;
                }

                case Pex::OpCode::ARRAY_CREATE:
                {
                    node = std::make_shared<Node::ArrayCreate>(ip, args[0].getId(), typeOfVar(args[0].getId()), fromValue(ip, args[1]));
                    break;
                }
                case Pex::OpCode::ARRAY_LENGTH:
                {
                    node = std::make_shared<Node::ArrayLength>(ip, args[0].getId(), fromValue(ip, args[1]));
                    break;
                }
                case Pex::OpCode::ARRAY_GETELEMENT:
                {
                    node = std::make_shared<Node::ArrayAccess>(ip, args[0].getId(), fromValue(ip, args[1]), fromValue(ip, args[2]));
                    break;
                }
                case Pex::OpCode::ARRAY_SETELEMENT:
                {
                    node = std::make_shared<Node::ArrayAccess>(ip, Pex::StringTable::Index(), fromValue(ip, args[0]), fromValue(ip, args[1]));
                    node = std::make_shared<Node::Assign>(ip, node, fromValue(ip, args[2]));
                    break;
                }
                case Pex::OpCode::ARRAY_FINDELEMENT:
                {
                    auto callNode = std::make_shared<Node::CallMethod>(ip, args[1].getId(), fromValue(ip, args[0]), m_TempTable.findIdentifier(("find")));
                    auto argNode = callNode->getParameters();
                    *argNode << fromValue(ip, args[2]);
                    *argNode << fromValue(ip, args[3]);
                    node = callNode;
                    break;
                }
                case Pex::OpCode::ARRAY_RFINDELEMENT:
                {
                    auto callNode = std::make_shared<Node::CallMethod>(ip, args[1].getId(), fromValue(ip, args[0]), m_TempTable.findIdentifier(("rfind")));
                    auto argNode = callNode->getParameters();
                    *argNode << fromValue(ip, args[2]);
                    *argNode << fromValue(ip, args[3]);

                    node = callNode;
                    break;
                }
                case Pex::OpCode::IS:
                {
                    node = std::make_shared<Node::BinaryOperator>(ip, 0, args[0].getId(), fromValue(ip, args[1]), "is", fromValue(ip, args[2]));
                    break;
                }
                case Pex::OpCode::STRUCT_CREATE:
                {
                    node = std::make_shared<Node::StructCreate>(ip, args[0].getId(), typeOfVar(args[0].getId()));
                    break;
                }
                case Pex::OpCode::STRUCT_GET:
                {
                    node = std::make_shared<Node::PropertyAccess>(ip, args[0].getId(), fromValue(ip, args[1]), args[2].getId());
                    break;
                }
                case Pex::OpCode::STRUCT_SET:
                {
                    node = std::make_shared<Node::PropertyAccess>(ip, Pex::StringTable::Index(), fromValue(ip, args[0]), args[1].getId());
                    node = std::make_shared<Node::Assign>(ip, node, fromValue(ip, args[2]));
                    break;
                }
                case Pex::OpCode::ARRAY_FINDSTRUCT:
                {
                    auto callNode = std::make_shared<Node::CallMethod>(ip, args[1].getId(), fromValue(ip, args[0]), m_TempTable.findIdentifier("findstruct"));
                    auto argNode = callNode->getParameters();
                    *argNode << fromValue(ip, args[2]);
                    *argNode << fromValue(ip, args[3]);
                    *argNode << fromValue(ip, args[4]);

                    node = callNode;
                    break;
                }
                case Pex::OpCode::ARRAY_RFINDSTRUCT:
                {
                    auto callNode = std::make_shared<Node::CallMethod>(ip, args[1].getId(), fromValue(ip, args[0]), m_TempTable.findIdentifier("rfindstruct"));
                    auto argNode = callNode->getParameters();
                    *argNode << fromValue(ip, args[2]);
                    *argNode << fromValue(ip, args[3]);
                    *argNode << fromValue(ip, args[4]);

                    node = callNode;
                    break;
                }
                case Pex::OpCode::ARRAY_ADD:
                {
                    auto callNode = std::make_shared<Node::CallMethod>(ip, Pex::StringTable::Index(), fromValue(ip, args[0]), m_TempTable.findIdentifier("add"));
                    auto argNode = callNode->getParameters();
                    *argNode << fromValue(ip, args[1]);
                    *argNode << fromValue(ip, args[2]);

                    node = callNode;
                    break;
                }
                case Pex::OpCode::ARRAY_INSERT:
                {
                    auto callNode = std::make_shared<Node::CallMethod>(ip, Pex::StringTable::Index(), fromValue(ip, args[0]), m_TempTable.findIdentifier("insert"));
                    auto argNode = callNode->getParameters();
                    *argNode << fromValue(ip, args[1]);
                    *argNode << fromValue(ip, args[2]);

                    node = callNode;
                    break;
                }
                case Pex::OpCode::ARRAY_REMOVELAST:
                {
                    node = std::make_shared<Node::CallMethod>(ip, Pex::StringTable::Index(), fromValue(ip, args[0]), m_TempTable.findIdentifier("removelast"));
                    break;
                }
                case Pex::OpCode::ARRAY_REMOVE:
                {
                    auto callNode = std::make_shared<Node::CallMethod>(ip, Pex::StringTable::Index(), fromValue(ip, args[0]), m_TempTable.findIdentifier("remove"));
                    auto argNode = callNode->getParameters();
                    *argNode << fromValue(ip, args[1]);
                    *argNode << fromValue(ip, args[2]);

                    node = callNode;
                    break;
                }
                case Pex::OpCode::ARRAY_CLEAR:
                {
                    node = std::make_shared<Node::CallMethod>(ip, Pex::StringTable::Index(), fromValue(ip, args[0]), m_TempTable.findIdentifier("clear"));
                    break;
                }
                case Pex::OpCode::ARRAY_GETALLMATCHINGSTRUCTS:
                {
                    auto callNode = std::make_shared<Node::CallMethod>(ip, args[1].getId(), fromValue(ip, args[0]), m_TempTable.findIdentifier("getallmatchingstructs"));
                    auto argNode = callNode->getParameters();
                    *argNode << fromValue(ip, args[2]);
                    *argNode << fromValue(ip, args[3]);
                    *argNode << fromValue(ip, args[4]);
                    *argNode << fromValue(ip, args[5]);

                    node = callNode;
                    break;
                }

            }
            if (node)
            {                
                *scope << checkAssign(node);
            }
        }
    }
}

/**
 * @brief Find the block containing a given instruction.
 * @param ip Indice of the instruction.
 * @return The indice of the containing block.
 */
size_t Decompiler::PscDecompiler::findBlockForInstruction(size_t ip)
{
    for (auto& bloc_kv : m_CodeBlocs)
    {
        auto& bloc = bloc_kv.second;
        if (bloc->getBegin() <= ip && ip <= bloc->getEnd())
        {
            return bloc->getBegin();
        }
    }
    return PscCodeBlock::END;
}

/**
 * @brief Rebuild the statement for each block.
 */
void Decompiler::PscDecompiler::rebuildExpressionsInBlocks()
{

    for (auto& bloc_kv : m_CodeBlocs)
    {
        auto& bloc = bloc_kv.second;

        auto scope = bloc->getScope();
        rebuildExpression(scope->shared_from_this());
    }
}

/**
 * @brief Rebuild statement in one block.
 *
 * The statements are reconstructed by propagating the first node
 * where the result computed by this node is used in the following
 * instructions.
 * @param scope Scope which will receive the nodes.
 */
void Decompiler::PscDecompiler::rebuildExpression(Node::BasePtr scope)
{
    auto it = scope->begin();
    while (it != scope->end())
    {
        auto nextIt = std::next(it);
        auto expressionGeneration = *it;
        if (! expressionGeneration->isFinal() && nextIt != scope->end())
        {
            auto expressionUse = *nextIt;

            // Check if an identifier in expressionUse references the result of expressionGeneration
            // If so, perform a replacement
            // At this steps of the decompilation, there should be only one replacement.
            auto modified = Node::WithNode<Node::Constant>()
                    .select([&] (Node::Constant* node) {
                        auto& value = node->getConstant();
                        return value.getType() == Pex::ValueType::Identifier && value.getId() == expressionGeneration->getResult();
                    })
                    .transform([&] (Node::Constant* node) {
                        (void)node;
                        return expressionGeneration;
                    })
                    .on(expressionUse);
            if (modified == 0)
            {
                std::advance(it, 1);
            }
            else if (modified == 1)
            {
                it = scope->begin();
            }
            else
            {
                throw std::runtime_error("Decompilation failed");
            }
        }
        else
        {
            std::advance(it, 1);
        }
    }
}

/**
 * @brief Rebuilds the boolean operations.
 *
 * This pass detects the patterns of the "or" and "and" operation generated by the
 * Papyrus compiler. The detection is performed from the startBlock until the endBlock is reached.
 * The endBlock is not processed.
 *
 * @param startBlock Indice of the first block to check
 * @param endBlock Indice of the block where to stop the detection.
 */
void Decompiler::PscDecompiler::rebuildBooleanOperators(size_t startBlock, size_t endBlock)
{
#ifdef TRACE_DECOMPILATION
    m_Log << "--- BEGIN REBUILD : " << startBlock << " " << endBlock << std::endl;
    dumpBlock(startBlock, endBlock);
#endif
    auto begin = m_CodeBlocs.find(startBlock);
    auto end = m_CodeBlocs.find(endBlock);
    auto it = begin;
    while (it != end)
    {
        auto& source = it->second;
        int advance = 1;
#ifdef TRACE_DECOMPILATION
        m_Log << "?" << source->getBegin() << " source->isConditional()=" << source->isConditional() <<" "
            << "source->getScope()->size()=" << source->getScope()->size() <<" "
            << '\n';
#endif
        // Process only conditional block with at least one statement.
        if (source->isConditional() && source->getScope()->size() != 0)
        {
#ifdef TRACE_DECOMPILATION
            m_Log << "??"             << "source->getCondition()=" << source->getCondition() << " "
                << "source->getScope()->back()->getResult()=" << source->getScope()->back()->getResult() << " "
                << '\n';
#endif
            // Ensure that the last statement computes the value of the condition variable.
            if (source->getCondition() == source->getScope()->back()->getResult())
            {
#ifdef TRACE_DECOMPILATION
            // AND ?
                m_Log << "AND? "
                      << "source->onTrue() == source->getEnd() + 1:" << (source->onTrue() == source->getEnd() + 1) << " "
                      << '\n';
                m_Log << "OR? "
                      << "source->onFalse() == source->getEnd() + 1:" << (source->onFalse() == source->getEnd() + 1) << " "
                      << '\n';
#endif
                // If the true block is the block directly following this one, it is a potential and
                bool maybeAnd = (source->onTrue() == source->getEnd() + 1);
                // If the false block is the block directly following this one, it is a potential or
                bool maybeOr = (source->onFalse() == source->getEnd() + 1);
                assert(!(maybeAnd && maybeOr));

                if (maybeAnd)
                {
                    // Rebuild the boolean operators between the true and false block.
                    rebuildBooleanOperators(source->onTrue(), source->onFalse());
                    auto & onTrue  = m_CodeBlocs[source->onTrue()];
                    auto & onFalse = m_CodeBlocs[source->onFalse()];
                    if (onTrue->getScope()->size() == 1)
                    {
#ifdef TRACE_DECOMPILATION
                        m_Log << "AND@" << source->getBegin() << " "
                              << "onTrue->getScope()->size() == 1:" << (onTrue->getScope()->size() == 1) << " "
                              << "onTrue->getScope()->back()->getResult() == source->getCondition():" << (onTrue->getScope()->back()->getResult() == source->getCondition()) << " "
                              << "\n";
#endif
                        // The true block computes the same value as the current block, and it is the condition variable
                        // This is a "and" operator
                        if(onTrue->getScope()->size() == 1 && onTrue->getScope()->back()->getResult() == source->getCondition())
                        {
                            // Create the binary "&&" operator.
                            auto left = source->getScope()->back();
                            source->getScope()->removeChild(left);

                            auto right = onTrue->getScope()->front();
                            onFalse->getScope()->removeChild(right);

                            auto andOperator = std::make_shared<Node::BinaryOperator>(-1, 7, source->getCondition(), left, "&&", right);
                            *(source->getScope()) << andOperator;

                            // Remove the true block now that the expression is rebuild
                            m_CodeBlocs.erase(onTrue->getBegin());

                            // Merge the false block.
                            source->getScope()->mergeChildren(onFalse->getScope()->shared_from_this());
                            rebuildExpression(source->getScope()->shared_from_this());
                            source->setEnd(onFalse->getEnd());
                            source->setCondition(onFalse->getCondition(), onFalse->onTrue(), onFalse->onFalse());
                            m_CodeBlocs.erase(onFalse->getBegin());

                            advance = 0;
#ifdef TRACE_DECOMPILATION
                            m_Log << "AND? " << "detected" << std::endl;
                            dumpBlock(source->getBegin(), source->getEnd()+1);
#endif
                        }
                    }
                    it = m_CodeBlocs.find(source->getBegin());

                }
                else if (maybeOr)
                {
                    // Rebuild the boolean operators between the false and true block.
                    rebuildBooleanOperators(source->onFalse(), source->onTrue());
                    auto & onTrue  = m_CodeBlocs[source->onTrue()];
                    auto & onFalse = m_CodeBlocs[source->onFalse()];
#ifdef TRACE_DECOMPILATION
                    m_Log << "OR@"  << source->getBegin() << " "
                          << "onFalse->getScope()->size() == 1:" << (onFalse->getScope()->size() == 1) << " "
                          << "onFalse->getScope()->back()->getResult() == source->getCondition():" << (onFalse->getScope()->back()->getResult() == source->getCondition()) << " "
                          << "\n";
#endif
                    // The false block computes the same value as the current block, and it is the condition variable
                    // This is a "or operator
                    if(onFalse->getScope()->size() == 1 && onFalse->getScope()->back()->getResult() == source->getCondition())
                    {
                        //Create the "||" operator
                        auto left = source->getScope()->back();
                        source->getScope()->removeChild(left);

                        auto right = onFalse->getScope()->front();
                        onFalse->getScope()->removeChild(right);

                        auto orOperator = std::make_shared<Node::BinaryOperator>(-1, 8, source->getCondition(), left, "||", right);
                        *(source->getScope()) << orOperator;

                        //Remove the false block now that the expression is rebuild
                        m_CodeBlocs.erase(onFalse->getBegin());

                        //Merge the true block.
                        source->getScope()->mergeChildren(onTrue->getScope()->shared_from_this());
                        rebuildExpression(source->getScope()->shared_from_this());
                        source->setEnd(onTrue->getEnd());
                        source->setCondition(onTrue->getCondition(), onTrue->onTrue(), onTrue->onFalse());
                        m_CodeBlocs.erase(onTrue->getBegin());
                        advance = 0;
#ifdef TRACE_DECOMPILATION
                        m_Log << "OR? " << "detected" << std::endl;
                        dumpBlock(source->getBegin(), source->getEnd()+1);
#endif
                    }
                    it = m_CodeBlocs.find(source->getBegin());
                    advance = 0;
                }
            }
        }
        std::advance(it, advance);
    }
#ifdef TRACE_DECOMPILATION
    m_Log << "--- END REBUILD : " << startBlock << " " << endBlock << std::endl;
    dumpBlock(startBlock, endBlock);
#endif
}

/**
 * @brief Rebuild the flow control statements
 *
 * This pass detects the pattern of the if and while statements. Once a pattern
 * has been detected, the nodes are recreated.
 *
 * @param startBlock Indice of the first block to check
 * @param endBlock Indice of the block where to stop the detection.
 * @return The statement tree representing the statements between the boundaries.
 */
Node::BasePtr Decompiler::PscDecompiler::rebuildControlFlow(size_t startBlock, size_t endBlock)
{
    if (endBlock < startBlock)
    {
        throw std::runtime_error("Decompilation failed");
    }
    auto begin = m_CodeBlocs.find(startBlock);
    auto end = m_CodeBlocs.find(endBlock);
    auto it = begin;

    Node::BasePtr result = std::make_shared<Node::Scope>();
    while (it != end)
    {
        auto current = it->first;
        auto& source = it->second;
        int advance = 1;
        // Check conditional blocks.
        if (source->isConditional())
        {
            auto exit = source->onFalse();
            // Find the block before the false block.
            auto beforeExit = findBlockForInstruction(exit-1);
            if (beforeExit == PscCodeBlock::END)
            {
                // Decompilation failed
                throw std::runtime_error("Decompilation failed");
            }
            auto& lastBlock = m_CodeBlocs[beforeExit];

            // The last block is an unconditional jump to the current block
            // This is a while.
            if (! lastBlock->isConditional() && lastBlock->getNext() == current)
            {
                // while loop
                auto whileStartBlock = source->onTrue();
                auto whileEndBlock = source->onFalse();


                Node::BasePtr whileCondition = std::make_shared<Node::Constant>(-1, Pex::Value(source->getCondition(), true));

                result->mergeChildren(source->getScope()->shared_from_this());

                // Rebuild the statements in the while loop.
                auto whileBody = rebuildControlFlow(whileStartBlock, whileEndBlock);

                *result << std::make_shared<Node::While>(-1, whileCondition, whileBody);

                advance = 0;
                it = m_CodeBlocs.find(whileEndBlock);
            }
            else if (!lastBlock->isConditional())
            {
                // The last block exits to the false block
                // This is a simple if
                if (lastBlock->getNext() == exit)
                {
                    // Simple If
                    auto ifStartBlock = source->onTrue();
                    auto ifEndBlock = source->onFalse();

                    Node::BasePtr ifCondition = std::make_shared<Node::Constant>(-1, Pex::Value(source->getCondition(), true));

                    result->mergeChildren(source->getScope()->shared_from_this());

                    // Rebuild the statements of the if body
                    auto ifBody = rebuildControlFlow(ifStartBlock, ifEndBlock);

                    *result << std::make_shared<Node::IfElse>(-1, ifCondition, ifBody, nullptr);

                    advance = 0;
                    it = m_CodeBlocs.find(ifEndBlock);
                }
                else // This is an if-else statement/
                {
                    auto ifStartBlock = source->onTrue();
                    auto elseStartBlock = source->onFalse();
                    auto endElseBlock = lastBlock->getNext();

                    Node::BasePtr ifCondition = std::make_shared<Node::Constant>(-1, Pex::Value(source->getCondition(), true));

                    result->mergeChildren(source->getScope()->shared_from_this());

                    // Rebuilds the statements in the if body.
                    auto ifBody = rebuildControlFlow(ifStartBlock, elseStartBlock);
                    // Rebuilds the statements in the else body.
                    auto elseBody = rebuildControlFlow(elseStartBlock, endElseBlock);

                    *result << std::make_shared<Node::IfElse>(-1, ifCondition, ifBody, elseBody);

                    advance = 0;
                    it = m_CodeBlocs.find(endElseBlock);
                }

            }

        }
        else
        {
            //On unconditional jump, merge the current block statements to the result scope.
            result->mergeChildren(source->getScope()->shared_from_this());
        }
        std::advance(it, advance);
    }
    rebuildExpression(result);
    return result;
}

/**
 * @brief Finds the lowest common scope for a variable's references.
 * @param var Name of the variable.
 * @param scope Initial enclosing scope.
 * @return The lowest common scope.
 */
Node::BasePtr Decompiler::PscDecompiler::findScopeForVariable(const Pex::StringTable::Index &var, Node::BasePtr scope)
{
    // Default result is the initial scope.
    Node::BasePtr result = scope;

    // Find all references to the variable.
    auto references = Node::WithNode<Node::Constant>()
            .select([&] (Node::Constant* node) {
                auto& val = node->getConstant();
                return val.getType() == Pex::ValueType::Identifier && val.getId() == var;
            })
            .from(scope);

    // If there are some references, we perform the scope detection
    if (references.size() != 0)
    {
        // List of scope in the hierarchycal order.
        std::deque<Node::BasePtr> commonScopes;

        // Initialize the scope hierarchy list with the first reference.
        auto initial = references.front();
        references.pop_front();
        while(initial)
        {
            // Find the scope hierarchy for this node by goin up,
            // collecting scope as we go.
            if (initial->is<Node::Scope>())
            {
                commonScopes.push_front(initial);
            }
            initial = initial->getParent();
        }

        // For each remaining reference, we look for a common scope
        for (auto ref : references)
        {
            auto it = commonScopes.end();
            // Exit if the scope was found in the current hierarchy
            while (ref && it == commonScopes.end())
            {
                // Find the enclosing scope of the reference.
                while(ref && !ref->is<Node::Scope>())
                {
                    ref = ref->getParent();
                }

                // Search the scope in the current hierarchy
                it = std::find(commonScopes.begin(), commonScopes.end(), ref);
                ref = ref->getParent();
            }

            // At least the initial scope should be common to all reference
            assert(it != commonScopes.end());

            // If the found scope is not the last of the hierarchy, we delete from this scope to the end.
            if (std::next(it) != commonScopes.end())
            {
                commonScopes.erase(std::next(it), commonScopes.end());
            }
        }
        // At least the initial scope should be common to all reference
        assert(commonScopes.size() > 0);
        // The result scop is the last from the hierarchy.
        result = commonScopes.back();
    }

    return result;
}

/**
 * @brief Handle the variable declaration.
 *
 * This pass finds the lowest common scope for a variable and adds a it's declaration
 * either on top, or on the first assignement if possible.
 *
 * @param program The program tree.
 */
void Decompiler::PscDecompiler::declareVariables(Node::BasePtr program)
{
    // For each variable declared in the function
    for (auto& local : m_Function.getLocals())
    {
        // Avoid temporary variables
        if(!isTempVar(local.getName()))
        {
            // Find the scope common to all reference of the variable
            auto scope = findScopeForVariable(local.getName(), program);
            assert(scope);

            auto declare = std::make_shared<Node::Declare>(-1, std::make_shared<Node::Constant>(-1, Pex::Value(local.getName(), true)), local.getTypeName());

            // Find all assignment to the variable
            auto assignments = Node::WithNode<Node::Assign>()
                    .select([&] (Node::Assign* node) {
                        if(node->getDestination()->is<Node::Constant>())
                        {
                            auto& value = node->getDestination()->as<Node::Constant>()->getConstant();

                            return value.getType() == Pex::ValueType::Identifier && value.getId() == local.getName();
                        }
                        return  false;
                    })
                    .from(scope);
            // The first assignment is in the upper level scope
            if (assignments.size() > 0 && assignments.front()->getParent() == scope)
            {
                // Declare and assign at the same time
                auto assign = assignments.front()->as<Node::Assign>();
                assign->setDestination(declare);
            }
            else
            {
                // Declare at the top of the scope
                scope->push_front(declare);
            }
        }
    }
}



/**
 * @brief Clean the reconstructed tree.
 *
 * This pass perform a cleanup of the reconstructed tree to remove superfluous statement
 * and to rebuild higher level statement not directly inferred from the instruction flow.
 * This include the != operator, or the if-elseif-else program structure.
 *
 * @param program The root node of the program tree.
 */
void Decompiler::PscDecompiler::cleanUpTree(Node::BasePtr program)
{    
    program->computeInstructionBounds();


    // Remove the copy node, which was used to assign to temporary variables.;
    Node::WithNode<Node::Copy>()
        .transform([&] (Node::Copy* node) { return node->getValue();})
        .on(program);

    // Remove casting a variable as it's own type as they are useless
    Node::WithNode<Node::Cast>()
        .select([&] (Node::Cast* node) {
            if (node->getValue()->is<Node::Constant>())
            {
                auto value = node->getValue()->as<Node::Constant>()->getConstant();
                if (value.getType() == Pex::ValueType::Identifier)
                {
                    return typeOfVar(value.getId()) == node->getType();
                }
            }
            return false;
        })
        .transform([&] (Node::Cast* node) {
            return node->getValue();
        })
        .on(program);

    // Remove casting none as Something as they are invalid
    Node::WithNode<Node::Cast>()
        .select([&] (Node::Cast* node) {
            if (node->getValue()->is<Node::Constant>())
            {
                auto value = node->getValue()->as<Node::Constant>()->getConstant();
                return value.getType() == Pex::ValueType::None;
            }
            return false;
        })
        .transform([&] (Node::Cast* node) {
            return node->getValue();
        })
        .on(program);


    // Replace the identifiers name index with a string value, unmangling names and property autovar
    Node::WithNode<Node::Constant>()
            .select([&] (Node::Constant* node) {
                return node->getConstant().getType() == Pex::ValueType::Identifier;
            })
            .transform([&] (Node::Constant* node) {
                return std::make_shared<Node::IdentifierString>(node->getBegin(), getVarName(node->getConstant().getId()));
            })
            .on(program);


    // Apply ! operator on == comparison
    Node::WithNode<Node::UnaryOperator>()
        .select([&] (Node::UnaryOperator* node) {
            if (node->getOperator() == "!" && node->getValue()->is<Node::BinaryOperator>())
            {
                auto op = node->getValue()->as<Node::BinaryOperator>();
                return op->getOperator() == "==";
            }
            return false;
        })
        .transform([&] (Node::UnaryOperator* node) {
            auto op = node->getValue()->as<Node::BinaryOperator>();
            auto result = std::make_shared<Node::BinaryOperator>(op->getBegin(), op->getPrecedence(), op->getResult(), op->getLeft(), "!=", op->getRight());
            result->includeInstruction(node->getEnd());
            return result;
        })
        .on(program);

    // Rebuild ElseIf structures
    Node::WithNode<Node::IfElse>()
        .select([&] (Node::IfElse* node) {
            auto elseNode = node->getElse();
            return elseNode->size() == 1 && elseNode->operator[](0)->is<Node::IfElse>();
        })
        .transform([&] (Node::IfElse* node) {
            auto childIfNode = node->getElse()->operator[](0);            

            node->setElse(childIfNode->as<Node::IfElse>()->getElse());
            childIfNode->as<Node::IfElse>()->setElse(std::make_shared<Node::Scope>());

            *node->getElseIf() << childIfNode;
            node->getElseIf()->mergeChildren(childIfNode->as<Node::IfElse>()->getElseIf());

            return node->shared_from_this();
        })
    .on(program);

    // Extract assign operator ( x = x + 1 => x += 1)
    Node::WithNode<Node::Assign>()
        .select([&] (Node::Assign* node) {
            auto result = false;
            auto destination = node->getDestination();
            if (node->getValue()->is<Node::BinaryOperator>())
            {
                auto binaryOp = node->getValue()->as<Node::BinaryOperator>();
                // ||= and &&= are not valid operators
                // a.b.c += 1 doesn't seems to compile.
                // so is array[x] += 1
                if (binaryOp->getOperator() != "||" && binaryOp->getOperator() != "&&"
                    && !node->getDestination()->is<Node::PropertyAccess>()
                    && !node->getDestination()->is<Node::ArrayAccess>()
                )
                {                    
                    auto left = binaryOp->getLeft();
                    return Node::isSameTree(destination, left);
                }
            }
            return result;
        })
        .transform([&] (Node::Assign* node) {
            auto binaryOp = node->getValue()->as<Node::BinaryOperator>();
            return std::make_shared<Node::AssignOperator>(node->getBegin(), node->getDestination(), binaryOp->getOperator() + "=", binaryOp->getRight());

        })
        .on(program);

    program->computeInstructionBounds();

}

#ifdef TRACE_DECOMPILATION
#include "DumpTree.hpp"
#endif

/**
 * @brief Generate the code from the program tree.
 *
 * @param program The tree to output as code.
 */
void Decompiler::PscDecompiler::generateCode(Node::BasePtr program)
{
    if (m_CommentAsm)
    {
        for (auto& local : m_Function.getLocals())
        {
            if(isTempVar(local.getName()))
            {
                std::ostringstream stream;
                stream << ";" << local.getTypeName() << " " << local.getName();
                push_back(stream.str());
            }
        }
        push_back("");
    }

    PscCodeGenerator codegen(this);
    program->visit(&codegen);
}

/**
 * @brief Extract the identifier from a value.
 * @param value The value containing the identifier.
 * @return The identifier.
 */
Pex::StringTable::Index Decompiler::PscDecompiler::toIdentifier(const Pex::Value &value) const
{
    assert(value.getType() == Pex::ValueType::Identifier);
    return value.getId();
}

/**
 * @brief Create a tree node from a value.
 * @param ip Indice of the instruction using the value.
 * @param value Value used as constant.
 * @return The constant node.
 */
Node::BasePtr Decompiler::PscDecompiler::fromValue(size_t ip, const Pex::Value &value) const
{
    return std::make_shared<Node::Constant>(ip, value);
}

/**
 * @brief Put an assign statement if needed by an expression
 * If the result of an expression is not assigned to a temporary variable, then an assign statement
 * must assign the result to the variable.
 * @param expression Expression to check
 * @return The node with an assign or the expression node.
 */
Node::BasePtr Decompiler::PscDecompiler::checkAssign(Node::BasePtr expression) const
{
    assert(expression);
    auto& result = expression->getResult();
    if (result.isValid() && !isTempVar(result))
    {
        return std::make_shared<Node::Assign>(expression->getBegin(), std::make_shared<Node::Constant>(expression->getBegin(), Pex::Value(result, true)), expression);
    }
    return expression;
}

#ifdef TRACE_DECOMPILATION
void Decompiler::PscDecompiler::dumpBlock(size_t startBlock, size_t endBlock)
{
    auto begin = m_CodeBlocs.find(startBlock);
    auto end = m_CodeBlocs.find(endBlock);
    auto it = begin;
    while(it != end)
    {

        auto& b = it->second;

        m_Log << "-------" << it->first << ": " << b->getBegin() << " " << b->getEnd() << '\n';

        for (auto i = b->getBegin(); i <= b->getEnd() && i <m_Function.getInstructions().size(); ++i)
        {
            auto& ins = m_Function.getInstructions()[i];
            m_Log << std::dec << std::setw(3) << std::setfill('0') << i << ":" << ins.getOpCodeName();
            for (auto& a : ins.getArgs())
            {
                m_Log << " " << a.toString();
            }
            for (auto& a : ins.getVarArgs())
            {
                m_Log << " " <<a.toString();
            }
            m_Log << '\n';
        }

        m_Log << "------- cond:" << b->getCondition() << " true:" << b->onTrue() << " false:" << b->onFalse() << std::endl;
        ++it;
    }
}
#endif
