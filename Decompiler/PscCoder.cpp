#include "PscCoder.hpp"

#include <boost/algorithm/string/case_conv.hpp>

#include <cassert>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <locale>
#include <map>
#include <string>

#include "PscDecompiler.hpp"

/**
 * @brief Constructor
 * Builds an object associated with an output writer.
 *
 * @param writer Pointer to the output writer. The ownership is transferred.
 */
Decompiler::PscCoder::PscCoder(Decompiler::OutputWriter *writer)  :
    Coder(writer),
    m_CommentAsm(false)
{
}

/**
 * @brief Default desctructor
 */
Decompiler::PscCoder::~PscCoder()
{
}

/**
 * @brief Decompile a PEX binary to a Papyrus file.
 * @param pex
 */
void Decompiler::PscCoder::code(const Pex::Binary &pex)
{
    for(auto& object : pex.getObjects())
    {
        writeObject(object, pex);
    }

}

/**
 * @brief Set the option to output Assembly instruction in comments
 * @param commentAsm True to write the comments.
 * @return A reference to this.
 */
Decompiler::PscCoder &Decompiler::PscCoder::outputAsmComment(bool commentAsm)
{
    m_CommentAsm = commentAsm;
    return *this;
}

/**
 * @brief Write an object contained in the binary.
 * @param object Object to decompile
 * @param pex Binary to decompile.
 */
void Decompiler::PscCoder::writeObject(const Pex::Object &object, const Pex::Binary &pex)
{
    auto stream = indent(0);
    stream <<"ScriptName " << object.getName().asString();
    if (! object.getParentClassName().asString().empty())
    {
        stream << " Extends " << object.getParentClassName().asString();
    }
    if (object.getConstFlag())
      stream << " Const";

    writeUserFlag(stream, object, pex);
    write(stream);

    writeDocString(0, object);

    if (object.getStructInfos().size()) {
        write("");
        write(";-- Structs -----------------------------------------");
        writeStructs(object, pex);
    }

    if (object.getProperties().size()) {
        write("");
        write(";-- Properties --------------------------------------");
        writeProperties(object, pex);
    }

    if (object.getVariables().size()) {
        write("");
        write(";-- Variables ---------------------------------------");
        writeVariables(object, pex);
    }

    writeStates(object, pex);
}

/**
* @brief Write the struct definitions stored in the object.
* @param object Object containing the struct definitions.
* @param pex Binary to decompile.
*/
void Decompiler::PscCoder::writeStructs(const Pex::Object& object, const Pex::Binary& pex) {
    for (auto& sInfo : object.getStructInfos()) {
        write(indent(0) << "Struct " << sInfo.getName());

        bool foundInfo = false;
        if (pex.getDebugInfo().getStructOrders().size()) {
            // If we have debug info, we have information on the order
            // they were in the original source file, so use that order.
            for (auto& sOrder : pex.getDebugInfo().getStructOrders()) {
                if (sOrder.getObjectName() == object.getName() && sOrder.getOrderName() == sInfo.getName()) {
                    if (sOrder.getNames().size() == sInfo.getMembers().size()) {
                        foundInfo = true;
                        for (auto& orderName : sOrder.getNames()) {
                            for (auto& member : sInfo.getMembers()) {
                                if (member.getName() == orderName) {
                                    writeStructMember(member, pex);
                                    goto ContinueOrder;
                                }
                            }
                            // If we get here, then we failed to find the struct
                            // member in the debug info :(
                            throw std::runtime_error("Unable to locate the struct member by the name of '" + orderName.asString() + "'");
                        ContinueOrder:
                            continue;
                        }
                    } else {
                        // This shouldn't happen, but it's possible that the
                        // debug info doesn't include all members of the struct,
                        // so write them in whatever order they are in the file.
                        break;
                    }
                }
            }
        }

        if (!foundInfo) {
            for (auto& mem : sInfo.getMembers())
                writeStructMember(mem, pex);
        }

        write(indent(0) << "EndStruct");
        write("");
    }
}

/**
* @brief Write the struct member passed in.
* @param member The member to output.
* @param pex Binary to decompile.
*/
void Decompiler::PscCoder::writeStructMember(const Pex::StructInfo::Member& member, const Pex::Binary& pex)
{
    auto stream = indent(1);
    stream << mapType(member.getTypeName().asString()) << " " << member.getName();

    if (member.getValue().getType() != Pex::ValueType::None) {
        stream << " = " << member.getValue().toString();
    }
    writeUserFlag(stream, member, pex);
    if (member.getConstFlag())
      stream << " Const";
    write(stream);
    writeDocString(1, member);
}

/**
 * @brief Write the properties definitions stored in the object.
 * @param object Object containing the properties definitions.
 * @param pex Binary to decompile.
 */
void Decompiler::PscCoder::writeProperties(const Pex::Object &object, const Pex::Binary &pex)
{
    bool foundInfo = false;
    if (pex.getDebugInfo().getPropertyGroups().size()) {
        size_t totalProperties = 0;
        // If we have debug info, we have information on the order
        // they were in the original source file, so use that order.
        for (auto& propGroup : pex.getDebugInfo().getPropertyGroups()) {
            if (propGroup.getObjectName() == object.getName()) {
                totalProperties += propGroup.getNames().size();
            }
        }

        if (totalProperties == object.getProperties().size()) {
            foundInfo = true;

            for (auto& propGroup : pex.getDebugInfo().getPropertyGroups()) {
                if (propGroup.getObjectName() == object.getName()) {
                    int propertyIndent = 0;
                    if (!propGroup.getGroupName().asString().empty()) {
                        auto stream = indent(0);
                        stream << "Group " << propGroup.getGroupName();
                        writeUserFlag(stream, propGroup, pex);
                        write(stream);
                        writeDocString(0, propGroup);
                        propertyIndent = 1;
                    }

                    for (auto& propName : propGroup.getNames()) {
                        for (auto& prop : object.getProperties()) {
                            if (prop.getName() == propName) {
                                writeProperty(propertyIndent, prop, object, pex);
                                goto ContinueOrder;
                            }
                        }
                        // If we get here, then we failed to find the struct
                        // member in the debug info :(
                        throw std::runtime_error("Unable to locate the property by the name of '" + propName.asString() + "' referenced in the debug info");
                    ContinueOrder:
                        continue;
                    }

                    if (!propGroup.getGroupName().asString().empty()) {
                        write(indent(0) << "EndGroup");
                        write("");
                    }
                }
            }
        }
    }

    if (!foundInfo) {
        for (auto& prop : object.getProperties())
            writeProperty(0, prop, object, pex);
    }
}

/**
* @brief Write the property definition.
* @param i The indent level.
* @param prop The property to write.
* @param object Object containing the properties definitions.
* @param pex Binary to decompile.
*/
void Decompiler::PscCoder::writeProperty(int i, const Pex::Property& prop, const Pex::Object &object, const Pex::Binary& pex)
{
    auto stream = indent(i);
    auto isAutoReadOnly = !prop.hasAutoVar() &&
                           prop.isReadable() &&
                          !prop.isWritable() &&
                           prop.getReadFunction().getInstructions().size() == 1 &&
                           prop.getReadFunction().getInstructions()[0].getOpCode() == Pex::OpCode::RETURN &&
                           prop.getReadFunction().getInstructions()[0].getArgs().size() == 1;
    stream << mapType(prop.getTypeName().asString()) << " Property " << prop.getName();
    if (prop.hasAutoVar()) {
        auto var = object.getVariables().findByName(prop.getAutoVarName());
        if (var == nullptr)
            throw std::runtime_error("Auto variable for property not found");

        auto initialValue = var->getDefaultValue();
        if (initialValue.getType() != Pex::ValueType::None)
            stream << " = " << initialValue.toString();
        stream << " Auto";

        // The flags defined on the variable must be set on the property
        writeUserFlag(stream, *var, pex);
        if (var->getConstFlag())
          stream << " Const";
    } else if (isAutoReadOnly) {
      stream << " = " << prop.getReadFunction().getInstructions()[0].getArgs()[0].toString();
      stream << " AutoReadOnly";
    }
    writeUserFlag(stream, prop, pex);
    write(stream);
    writeDocString(i, prop);

    if (!prop.hasAutoVar() && !isAutoReadOnly) {
        if (prop.isReadable())
            writeFunction(i + 1, prop.getReadFunction(), object, pex, "Get");
        if (prop.isWritable())
            writeFunction(i + 1, prop.getWriteFunction(), object, pex, "Set");
        write(indent(i) << "EndProperty");
    }
}

/**
 * @brief Write the variables stored in the object.
 * @param object Object containing the variables.
 * @param pex Binary to decompile.
 */
void Decompiler::PscCoder::writeVariables(const Pex::Object &object, const Pex::Binary &pex)
{
    for (auto& var : object.getVariables())
    {
        auto name = var.getName().asString();
        bool compilerGenerated = (name.size() > 2 && name[0] == ':' && name[1] == ':');
        auto stream = indent(0);

        if (compilerGenerated)
            stream << "; ";

        stream << mapType(var.getTypeName().asString()) << " " << name;
        auto initialValue = var.getDefaultValue();
        if (initialValue.getType() != Pex::ValueType::None)
        {
            stream << " = " << initialValue.toString();
        }
        writeUserFlag(stream, var, pex);
        if (var.getConstFlag())
          stream << " Const";

        if (m_CommentAsm || !compilerGenerated)
        {
            write(stream);
        }
    }
}


/**
 * @brief Write the states contained in the object.
 * @param object Object containing the states.
 * @param pex Binary to decompile.
 */
void Decompiler::PscCoder::writeStates(const Pex::Object &object, const Pex::Binary &pex)
{
    for (auto& state : object.getStates())
    {
        auto name = state.getName().asString();
        if (name.empty())
        {
            if (state.getFunctions().size()) {
                write("");
                write(";-- Functions ---------------------------------------");
                writeFunctions(0, state, object, pex);
            }
        }
        else
        {
            write("");
            write(";-- State -------------------------------------------");
            auto stream = indent(0);

            // The auto state name canbe a different index than the state name, event if it is the same value.
            if (_stricmp(state.getName().asString().c_str(), object.getAutoStateName().asString().c_str()) == 0)
            {
                stream << "Auto ";
            }
            write(stream << "State " << state.getName().asString());
            writeFunctions(1, state, object, pex);
            write(indent(0) << "EndState");
        }
    }
}

/**
 * @brief Writes the functions associated with a state.
 * @param i The indentation level.
 * @param state State containing the functions.
 * @param object Object Containing the states.
 * @param pex Binary to decompile.
 */
void Decompiler::PscCoder::writeFunctions(int i, const Pex::State &state, const Pex::Object& object, const Pex::Binary &pex)
{
    for (auto& func : state.getFunctions())
    {
        write("");
        writeFunction(i, func, object, pex);
    }
}

/**
 * @brief Decompile a function.
 * @param i The indentation level.
 * @param function The function to decompile.
 * @param object The Object containing the function.
 * @param pex Binary to decompile.
 * @param name Name of the function. This parameter override the name stored in the function object.
 */
void Decompiler::PscCoder::writeFunction(int i, const Pex::Function &function, const Pex::Object& object, const Pex::Binary &pex, const std::string &name)
{
    std::string functionName = name;

    if (functionName.empty())
    {
        functionName = function.getName().asString();
    }

    bool isEvent = functionName.size() > 2 && !_stricmp(functionName.substr(0, 2).c_str(), "on");
    if (functionName.size() > 9 && !_stricmp(functionName.substr(0, 9).c_str(), "::remote_")) {
      isEvent = true;
      functionName = functionName.substr(9);
      functionName[function.getParams()[0].getTypeName().asString().size()] = '.';
    }

    if (functionName != "GetState" && functionName != "GotoState")
    {
        auto stream = indent(i);
        if (_stricmp(function.getReturnTypeName().asString().c_str(), "none") != 0)
            stream << mapType(function.getReturnTypeName().asString()) << " ";

        if (isEvent)
          stream << "Event ";
        else
          stream << "Function ";
        stream << functionName << "(";

        auto first = true;
        for (auto& param : function.getParams())
        {
            if (first)
            {
                first = false;
            }
            else
            {
                stream << ", ";
            }
            stream << mapType(param.getTypeName().asString()) << " " << param.getName();
        }
        stream << ")";

        if (function.isGlobal())
        {
            stream << " Global";
        }
        if (function.isNative())
        {
            stream << " Native";
        }
        writeUserFlag(stream, function, pex);
        write(stream);

        writeDocString(i, function);

        if (! function.isNative())
        {
            for (auto& line : PscDecompiler(function, object, m_CommentAsm))
            {
                write(indent(i+1) << line);
            }
            if (isEvent)
              write(indent(i) << "EndEvent");
            else
              write(indent(i) << "EndFunction");
        }
    }
    else
    {
        write(indent(i) << "; Skipped compiler generated " << functionName);
    }
}

/**
 * @brief Write the user flags associated with an item.
 * @param stream Stream to write the flags to.
 * @param flagged Flagged item.
 * @param pex Binary to decompile.
 */
void Decompiler::PscCoder::writeUserFlag(std::ostream& stream, const Pex::UserFlagged &flagged, const Pex::Binary &pex)
{
    auto flags = flagged.getUserFlags();
    for (auto& flag : pex.getUserFlags())
    {
        if (flags & flag.getFlagMask())
        {
            stream << " " << flag.getName();
        }
    }
}

/**
 * @brief Write the documentation string of an item.
 * @param i Indentation level.
 * @param item Documented item.
 */
void Decompiler::PscCoder::writeDocString(int i, const Pex::DocumentedItem &item)
{
    if (! item.getDocString().asString().empty())
    {
        write(indent(i) << "{ " << item.getDocString().asString() << " }");
    }
}

static const std::map<std::string, std::string> prettyTypeNameMap {
    // Builtin Types
    { "bool", "Bool" },
    { "float", "Float" },
    { "int", "Int" },
    { "string", "String" },
    { "var", "Var" },

    // Special
    { "self", "Self" },

    // General Types
    { "action", "Action" },
    { "activator", "Activator" },
    { "activemagiceffect", "ActiveMagicEffect" },
    { "actor", "Actor" },
    { "actorbase", "ActorBase" },
    { "actorvalue", "ActorValue" },
    { "alias", "Alias" },
    { "ammo", "Ammo" },
    { "apparatus", "Apparatus" },
    { "armor", "Armor" },
    { "associationtype", "AssociationType" },
    { "book", "Book" },
    { "cell", "Cell" },
    { "class", "Class" },
    { "constructibleobject", "ConstructibleObject" },
    { "container", "Container" },
    { "debug", "Debug" },
    { "door", "Door" },
    { "effectshader", "EffectShader" },
    { "enchantment", "Enchantment" },
    { "encounterzone", "EncounterZone" },
    { "explosion", "Explosion" },
    { "faction", "Faction" },
    { "flora", "Flora" },
    { "form", "Form" },
    { "formlist", "FormList" },
    { "furniture", "Furniture" },
    { "game", "Game" },
    { "globalvariable", "GlobalVariable" },
    { "hazard", "Hazard" },
    { "idle", "Idle" },
    { "imagespacemodifier", "ImageSpaceModifier" },
    { "impactdataset", "ImpactDataSet" },
    { "ingredient", "Ingredient" },
    { "key", "Key" },
    { "keyword", "Keyword" },
    { "leveledactor", "LeveledActor" },
    { "leveleditem", "LeveledItem" },
    { "leveledspell", "LeveledSpell" },
    { "light", "Light" },
    { "location", "Location" },
    { "locationalias", "LocationAlias" },
    { "locationreftype", "LocationRefType" },
    { "magiceffect", "MagicEffect" },
    { "math", "Math" },
    { "message", "Message" },
    { "miscobject", "MiscObject" },
    { "musictype", "MusicType" },
    { "objectreference", "ObjectReference" },
    { "outfit", "Outfit" },
    { "package", "Package" },
    { "perk", "Perk" },
    { "potion", "Potion" },
    { "projectile", "Projectile" },
    { "quest", "Quest" },
    { "race", "Race" },
    { "referencealias", "ReferenceAlias" },
    { "refcollectionalias", "RefCollectionAlias" },
    { "scene", "Scene" },
    { "scroll", "Scroll" },
    { "scriptobject", "ScriptObject" },
    { "shout", "Shout" },
    { "soulgem", "SoulGem" },
    { "sound", "Sound" },
    { "soundcategory", "SoundCategory" },
    { "spell", "Spell" },
    { "static", "Static" },
    { "talkingactivator", "TalkingActivator" },
    { "topic", "Topic" },
    { "topicinfo", "TopicInfo" },
    { "utility", "Utility" },
    { "visualeffect", "VisualEffect" },
    { "voicetype", "VoiceType" },
    { "weapon", "Weapon" },
    { "weather", "Weather" },
    { "wordofpower", "WordOfPower" },
    { "worldspace", "WorldSpace" },
};

/**
* @brief Map the type name used by the compiler to the form most used by people.
* @param type The type to map.
*/
std::string Decompiler::PscCoder::mapType(std::string type)
{
    if (type.length() > 2 && type[type.length() - 2] == '[' && type[type.length() - 1] == ']')
        return mapType(type.substr(0, type.length() - 2)) + "[]";
    auto lowerType = type;
    boost::algorithm::to_lower(lowerType);
    auto a = prettyTypeNameMap.find(lowerType);
    if (a != prettyTypeNameMap.end())
        return a->second;
    return type;
}