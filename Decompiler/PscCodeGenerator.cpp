#include "PscCodeGenerator.hpp"

#include <cassert>

#include "Node/WithNode.hpp"

static inline
bool isTempVar(const Pex::StringTable::Index& var)
{
    auto& name = var.asString();
    return name.length() > 6 && name.substr(0, 6) == "::temp" && name.substr(name.length() - 4, 4) != "_var";
}

static inline
std::string getVarName(const Pex::StringTable::Index& var)
{
    auto& name = var.asString();
    if (name.length() > 6 && name.substr(0, 2) == "::" && name.substr(name.length() - 4, 4) == "_var")
    {
        return name.substr(2, name.length() - 6);
    }

    return name;
}


Decompiler::PscCodeGenerator::PscCodeGenerator(Decompiler::PscDecompiler* decompiler) :
    m_Level(0),
    m_Decompiler(decompiler)
{
    assert(decompiler);
}

Decompiler::PscCodeGenerator::~PscCodeGenerator()
{

}
/*
std::ostringstream& Disassembler::PscCodeGenerator::indent()
{
    std::ostringstream result;
    return result;
}
*/
void Decompiler::PscCodeGenerator::newLine()
{
    m_Decompiler->push_back(m_Result.str());
    m_Result = std::ostringstream();
    for (auto i = 0; i < m_Level; ++i)
    {
        m_Result << '\t';
    }
}

void Decompiler::PscCodeGenerator::visit(Node::Scope* node)
{
    auto not_first = false;
    for(auto statement : *node)
    {
        if(not_first)
        {
            newLine();
        }
        else
        {
            not_first = true;
        }
        if (statement->getBegin() != -1 &&  statement->getEnd() != -1)
        {
            m_Decompiler->decodeToAsm(m_Level, statement->getBegin(), statement->getEnd());
        }
        statement->visit(this);
    }
    if (node->getParent() == nullptr)
    {
        newLine();
    }
}

void Decompiler::PscCodeGenerator::visit(Node::BinaryOperator* node)
{
    bool parenOnLeft = node->getPrecedence() < node->getLeft()->getPrecedence();
    bool parenOnRight = node->getPrecedence() < node->getRight()->getPrecedence();
    if (parenOnLeft)
    {
        m_Result << "(";
    }
    node->getLeft()->visit(this);
    if (parenOnLeft)
    {
        m_Result << ")";
    }
    m_Result << " " << node->getOperator() << " ";
    if (parenOnRight)
    {
        m_Result << "(";
    }
    node->getRight()->visit(this);
    if (parenOnRight)
    {
        m_Result << ")";
    }
}

void Decompiler::PscCodeGenerator::visit(Node::UnaryOperator* node)
{
    bool paren = node->getPrecedence() < node->getValue()->getPrecedence();

    m_Result << node->getOperator();
    if (paren)
    {
        m_Result << "(";
    }
    node->getValue()->visit(this);
    if (paren)
    {
        m_Result << ")";
    }
}
void Decompiler::PscCodeGenerator::visit(Node::Assign* node)
{
    node->getDestination()->visit(this);
    m_Result << " = ";
    node->getValue()->visit(this);
}

void Decompiler::PscCodeGenerator::visit(Node::AssignOperator* node)
{
    node->getDestination()->visit(this);
    m_Result << " " << node->getOperator() << " ";
    node->getValue()->visit(this);
}

void Decompiler::PscCodeGenerator::visit(Node::Copy *node)
{
    node->getValue()->visit(this);
}

void Decompiler::PscCodeGenerator::visit(Node::Cast* node)
{
    bool paren = node->getPrecedence() < node->getValue()->getPrecedence() || node->getValue()->is<Node::Cast>();

    if (paren)
    {
        m_Result << "(";
    }
    node->getValue()->visit(this);
    if (paren)
    {
        m_Result << ")";
    }
    m_Result << " as " << node->getType();
}

void Decompiler::PscCodeGenerator::visit(Node::CallMethod* node)
{
    bool paren = node->getPrecedence() < node->getObject()->getPrecedence();

    if (paren)
    {
        m_Result << "(";
    }
    node->getObject()->visit(this);
    if (paren)
    {
        m_Result << ")";
    }
    m_Result << "." << node->getMethod() << "(";
    node->getParameters()->visit(this);
    m_Result << ")";
}

void Decompiler::PscCodeGenerator::visit(Node::Params *node)
{
    bool not_first = false;
    for (auto param : *node)
    {
        if (not_first)
        {
            m_Result << ", ";
        }
        else
        {
            not_first = true;
        }
        param->visit(this);
    }
}

void Decompiler::PscCodeGenerator::visit(Node::Return* node)
{
    m_Result << "return ";
    if (node->getValue())
    {
        node->getValue()->visit(this);
    }
}

void Decompiler::PscCodeGenerator::visit(Node::PropertyAccess *node)
{
    bool paren = node->getPrecedence() < node->getObject()->getPrecedence();

    if (paren)
    {
        m_Result << "(";
    }
    node->getObject()->visit(this);
    if (paren)
    {
        m_Result << ")";
    }
    m_Result << "." << node->getProperty();
}

void Decompiler::PscCodeGenerator::visit(Node::ArrayCreate* node)
{
    std::string type = node->getType().asString();
    m_Result << "new " << type.substr(0, type.length() - 2) << "[" << node->getSize() << "]";
}

void Decompiler::PscCodeGenerator::visit(Node::ArrayLength* node)
{
    bool paren = node->getPrecedence() < node->getArray()->getPrecedence();

    if (paren)
    {
        m_Result << "(";
    }
    node->getArray()->visit(this);
    if (paren)
    {
        m_Result << ")";
    }
    m_Result << ".length";
}

void Decompiler::PscCodeGenerator::visit(Node::ArrayAccess *node)
{
    bool paren = node->getPrecedence() < node->getArray()->getPrecedence();

    if (paren)
    {
        m_Result << "(";
    }
    node->getArray()->visit(this);
    if (paren)
    {
        m_Result << ")";
    }
    m_Result << "[";
    node->getIndex()->visit(this);
    m_Result << "]";
}

void Decompiler::PscCodeGenerator::visit(Node::Constant* node)
{
    auto& value = node->getConstant();

    m_Result << value.toString();
}

void Decompiler::PscCodeGenerator::visit(Node::IdentifierString *node)
{
    m_Result << node->getIdentifier();
}

void Decompiler::PscCodeGenerator::visit(Node::While* node)
{
    m_Result << "while ";
    node->getCondition()->visit(this);
    m_Level++;
    newLine();
    node->getBody()->visit(this);
    m_Level--;
    newLine();
    if (node->getBody()->size() != 0)
    {
        m_Decompiler->decodeToAsm(m_Level, node->getBody()->back()->getEnd() + 1, node->getBody()->back()->getEnd() + 1);
    }
    m_Result << "endWhile";
}

void Decompiler::PscCodeGenerator::visit(Node::IfElse* node)
{
    m_Result << "if ";
    node->getCondition()->visit(this);
    m_Level++;
    newLine();
    node->getBody()->visit(this);
    m_Level--;
    newLine();
    auto lastBody = node->getBody();

    for (auto childNode : *(node->getElseIf()))
    {
        m_Decompiler->decodeToAsm(m_Level, childNode->getBegin()-1, childNode->getEnd());
        auto elseIf = childNode->as<Node::IfElse>();
        m_Result << "elseIf ";
        elseIf->getCondition()->visit(this);
        m_Level++;
        newLine();
        elseIf->getBody()->visit(this);
        m_Level--;
        newLine();
        lastBody = elseIf->getBody();
    }
    m_Decompiler->decodeToAsm(m_Level, lastBody->getEnd() + 1, lastBody->getEnd() + 1);
    if (node->getElse()->size() != 0)
    {
        m_Result << "else";
        m_Level++;
        newLine();
        node->getElse()->visit(this);
        m_Level--;
        newLine();
    }
    m_Result << "endIf";
}

void Decompiler::PscCodeGenerator::visit(Node::Declare *node)
{
    m_Result << node->getType() << " ";
    node->getObject()->visit(this);
}

