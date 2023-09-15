#include "PscCodeGenerator.hpp"

#include <cassert>

#include "Node/Nodes.hpp"
#include "PscCoder.hpp"

static bool isTempVar(const Pex::StringTable::Index& var)
{
    auto& name = var.asString();
    return name.length() > 6 && name.substr(0, 6) == "::temp" && name.substr(name.length() - 4, 4) != "_var";
}

static std::string getVarName(const Pex::StringTable::Index& var)
{
    auto& name = var.asString();
    if (name.length() > 6 && name.substr(0, 2) == "::" && name.substr(name.length() - 4, 4) == "_var")
    {
        return name.substr(2, name.length() - 6);
    }

    return name;
}


Decompiler::PscCodeGenerator::PscCodeGenerator(Decompiler::PscDecompiler* decompiler) :
    m_Decompiler(decompiler)
{
    assert(decompiler);
}


void Decompiler::PscCodeGenerator::writeDebugInfoLineNumbers(int64_t begin, int64_t end)
{
    auto result = getDebugInfoLineNumbers(begin, end);
    if (result.empty())
        return;

    m_Result << " ; #DEBUG_LINE_NO:";

    for (auto i = 0; i < result.size(); ++i)
    {
        if (i == 0)
        {
            m_Result << result[i];
        }
        else
        {
            m_Result << "," << result[i];
        }
    }
}

void Decompiler::PscCodeGenerator::newLine()
{

    if (!m_ExperimentalSyntaxWarning.empty()) {
        m_Result << " " << Decompiler::WARNING_COMMENT_PREFIX << " WARNING: Experimental syntax, may be incorrect: ";
        for (auto warn: m_ExperimentalSyntaxWarning){
            m_Result << warn << " ";
        }
        m_ExperimentalSyntaxWarning.clear();
    }
    auto nums = getDebugInfoLineNumbers(minIpForCurrentLine, maxIpForCurrentLine);
    writeDebugInfoLineNumbers(minIpForCurrentLine, maxIpForCurrentLine);
    resetIpsForCurrentLine();
    m_Decompiler->push_back(m_Result.str());
    m_Decompiler->addLineMapping(m_Decompiler->size() - 1, nums);

    m_Result = std::ostringstream();
    for (auto i = 0; i < m_Level; ++i)
    {
        m_Result << ' ' << ' ';
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
    addIpRangeForCurrentLine(node->getBegin(), node->getEnd());
    bool parenOnLeft = node->getPrecedence() < node->getLeft()->getPrecedence();
    if (node->getLeft()->is<Node::BinaryOperator>()) {
        auto l = node->getLeft()->as<Node::BinaryOperator>();
        if (l->getLeft()->is<Node::Cast>() || l->getRight()->is<Node::Cast>()) {
            parenOnLeft = true;
        }
    }
    bool parenOnRight = node->getPrecedence() < node->getRight()->getPrecedence();
    if (node->getRight()->is<Node::BinaryOperator>()) {
        auto r = node->getRight()->as<Node::BinaryOperator>();
        if (r->getLeft()->is<Node::Cast>() || r->getRight()->is<Node::Cast>()) {
            parenOnRight = true;
        }
    }
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
    if (node->getOperator() == "is" && node->getRight()->is<Node::IdentifierString>()) {
      m_Result << PscCoder::mapType(node->getRight()->as<Node::IdentifierString>()->getIdentifier());
    } else {
      node->getRight()->visit(this);
    }
    if (parenOnRight)
    {
        m_Result << ")";
    }
}

void Decompiler::PscCodeGenerator::visit(Node::UnaryOperator* node)
{
    addIpRangeForCurrentLine(node->getBegin(), node->getEnd());
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
    addIpRangeForCurrentLine(node->getBegin(), node->getEnd());
    node->getDestination()->visit(this);
    m_Result << " = ";
    node->getValue()->visit(this);
}

void Decompiler::PscCodeGenerator::visit(Node::AssignOperator* node)
{
    addIpRangeForCurrentLine(node->getBegin(), node->getEnd());
    node->getDestination()->visit(this);
    m_Result << " " << node->getOperator() << " ";
    node->getValue()->visit(this);
}

void Decompiler::PscCodeGenerator::visit(Node::Copy *node)
{
    addIpRangeForCurrentLine(node->getBegin(), node->getEnd());
    node->getValue()->visit(this);
}

void Decompiler::PscCodeGenerator::visit(Node::Cast* node)
{
    addIpRangeForCurrentLine(node->getBegin(), node->getEnd());
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
    m_Result << " as " << PscCoder::mapType(node->getType().asString());
}

void Decompiler::PscCodeGenerator::visit(Node::CallMethod* node)
{
    addIpRangeForCurrentLine(node->getBegin(), node->getEnd());
    bool paren = node->getPrecedence() < node->getObject()->getPrecedence();

    if (paren)
    {
        m_Result << "(";
    }
    if (node->getObject()->is<Node::IdentifierString>()) {
        m_Result << PscCoder::mapType(node->getObject()->as<Node::IdentifierString>()->getIdentifier());
    } else {
        node->getObject()->visit(this);
    }
    if (paren)
    {
        m_Result << ")";
    }
    m_Result << "." << node->getMethod() << "(";
    node->getParameters()->visit(this);
    m_Result << ")";
    if (node->isExperimentalSyntax()) {
        m_ExperimentalSyntaxWarning.push_back(node->getMethod().asString());
    }
}

void Decompiler::PscCodeGenerator::visit(Node::Params *node)
{
    addIpRangeForCurrentLine(node->getBegin(), node->getEnd());
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
    addIpRangeForCurrentLine(node->getBegin(), node->getEnd());
    m_Result << "Return ";
    if (node->getValue())
    {
        node->getValue()->visit(this);
    }
}

void Decompiler::PscCodeGenerator::visit(Node::PropertyAccess *node) {
    addIpRangeForCurrentLine(node->getBegin(), node->getEnd());
    bool paren = node->getPrecedence() < node->getObject()->getPrecedence();

    if (paren) {
        m_Result << "(";
    }
    node->getObject()->visit(this);
    if (paren)
    {
        m_Result << ")";
    }
    m_Result << "." << node->getProperty();
}

void Decompiler::PscCodeGenerator::visit(Node::StructCreate* node)
{
    addIpRangeForCurrentLine(node->getBegin(), node->getEnd());
    m_Result << "new " << PscCoder::mapType(node->getType().asString());
}

void Decompiler::PscCodeGenerator::visit(Node::ArrayCreate* node)
{
    addIpRangeForCurrentLine(node->getBegin(), node->getEnd());
    std::string type = PscCoder::mapType(node->getType().asString());
    m_Result << "new " << type.substr(0, type.length() - 2) << "[";

    node->getIndex()->visit(this);
    m_Result << "]";
}

void Decompiler::PscCodeGenerator::visit(Node::ArrayLength* node)
{
    addIpRangeForCurrentLine(node->getBegin(), node->getEnd());
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
    m_Result << ".Length";
}

void Decompiler::PscCodeGenerator::visit(Node::ArrayAccess *node)
{
    addIpRangeForCurrentLine(node->getBegin(), node->getEnd());
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
    addIpRangeForCurrentLine(node->getBegin(), node->getEnd());
    auto& value = node->getConstant();

    m_Result << value.toString();
}

void Decompiler::PscCodeGenerator::visit(Node::IdentifierString *node)
{
    addIpRangeForCurrentLine(node->getBegin(), node->getEnd());
    if (node->getIdentifier() == "self")
        m_Result << "Self";
    else
        m_Result << node->getIdentifier();
}

void Decompiler::PscCodeGenerator::visit(Node::While* node)
{
    m_Result << "While ";
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
    // EndWhile does not have valid debug line numbers, reset minIpForCurrentLine and maxIpForCurrentLine
    resetIpsForCurrentLine();
    m_Result << "EndWhile";
}

void Decompiler::PscCodeGenerator::visit(Node::IfElse* node)
{
    addIpRangeForCurrentLine(node->getBegin(), node->getEnd());
    auto cond = node->getCondition();
    m_Result << "If ";
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
        m_Result << "ElseIf ";
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
        m_Result << "Else";
        m_Level++;
        // Else do not have valid debug line numbers, reset minIpForCurrentLine and maxIpForCurrentLine
        resetIpsForCurrentLine();
        newLine();
        node->getElse()->visit(this);
        m_Level--;
        newLine();
    }
    // Endif does not have valid debug line numbers, reset minIpForCurrentLine and maxIpForCurrentLine
    resetIpsForCurrentLine();
    m_Result << "EndIf";
}

void Decompiler::PscCodeGenerator::visit(Node::Declare *node)
{
    addIpRangeForCurrentLine(node->getEnd(), node->getEnd());
    m_Result << PscCoder::mapType(node->getType().asString()) << " ";
    node->getObject()->visit(this);
}

void Decompiler::PscCodeGenerator::visit(Node::GuardStatement *node) {
    m_Result << "Guard ";
    addIpRangeForCurrentLine(node->getBegin(), node->getBegin());
    node->getParameters()->visit(this);
    m_Level++;
    // TODO: VERIFY: Remove this when syntax is verified
    m_ExperimentalSyntaxWarning.push_back("Guard");
    newLine();
    node->getBody()->visit(this);
    m_Level--;
    newLine();
    m_Result << "EndGuard";
    m_ExperimentalSyntaxWarning.push_back("EndGuard");
}

void Decompiler::PscCodeGenerator::visit(Node::TryGuard *node) {
    m_Result << "TryGuard ";
    addIpRangeForCurrentLine(node->getBegin(), node->getBegin());
    node->getParameters()->visit(this);
    m_Level++;
    // TODO: VERIFY: Remove this when syntax is verified
    m_ExperimentalSyntaxWarning.push_back("TryGuard");
    newLine();
    node->getBody()->visit(this);
    m_Level--;
    newLine();
    m_Result << "EndGuard";
    m_ExperimentalSyntaxWarning.push_back("EndGuard");

}

void Decompiler::PscCodeGenerator::visit(Node::EndGuard *node) {
    // NONE
}

void Decompiler::PscCodeGenerator::addIpRangeForCurrentLine(int64_t begin, int64_t end) {
    if (begin <= -1 || end <= -1) {
        return;
    }
    if (minIpForCurrentLine <= -1 || begin < minIpForCurrentLine) {
        minIpForCurrentLine = begin;
    }
    if (maxIpForCurrentLine <= -1 || end > maxIpForCurrentLine) {
        maxIpForCurrentLine = end;
    }
}

void Decompiler::PscCodeGenerator::resetIpsForCurrentLine() {
    minIpForCurrentLine = -1;
    maxIpForCurrentLine = -1;
}

std::vector<int64_t> Decompiler::PscCodeGenerator::getDebugInfoLineNumbers(int64_t begin, int64_t end) {
    if (begin < 0 || end < 0){
        return {};
    }
    auto & debugInfo = m_Decompiler->getDebugInfo();
    auto & lineNumbers = debugInfo.getLineNumbers();
    // no debug info
    if (lineNumbers.empty() || begin > lineNumbers.size() - 1) {
        return {};
    }
    std::vector<int64_t> result;
    int64_t line = -1;
    for (auto i = begin; i <= end; ++i)
    {
        if (i < lineNumbers.size() && lineNumbers[i] != line)
        {
            line = lineNumbers[i];
            result.push_back(line);
        }
    }
    return result;
}

