#pragma once

#include <iostream>
#include <vector>
#include "antlr4-runtime.h"
#include "PostLexer.h"
#include "PostParser.h"
#include "PostBaseVisitor.h"
#include "PostBaseListener.h"

#include "identifier.h"
#include "stable.h"

#include "utils.h"
#include "error.h"

using namespace antlr4;
using antlrcpp::Any;

class Compiler : PostBaseVisitor {

  public:
    Compiler() {}
    ~Compiler() {}

    void Process(ProcessedCode* code_, Error* err_);
    void EnableGraph(bool enable) { graphing = enable; }

  private:

    tree::ParseTreeProperty<string> storage;

    ProcessedCode* code;
    Error* err;
    SymbolTable* globalTable;
    SymbolTable* currentScope;
    // TODO -- This should probably be refactored later to not suck
    std::vector<Identifier*> currentId;
    std::vector<Type*> currentType;

    Graph graph;
    bool graphing = false;
    bool inherit = false;
    bool func_decl_err;
    int unnamed_inc = 0;

    void addRuleErr(ParserRuleContext* rule, string errmess);

    Any visitParse(PostParser::ParseContext* ctx) override;
    // Any visitTopDecl(PostParser::TopDeclContext* ctx) override;
    // Any visitTopFunc(PostParser::TopFuncContext* ctx) override;
    // Any visitBlockDecl(PostParser::BlockDeclContext* ctx) override;
    // Any visitBlockStat(PostParser::BlockStatContext* ctx) override;
    Any visitStat_compound(PostParser::Stat_compoundContext* ctx) override;
    Any visitParamList(PostParser::ParamListContext* ctx) override;
    Any visitTypeStd(PostParser::TypeStdContext* ctx) override;
    Any visitTypeStructUnion(PostParser::TypeStructUnionContext* ctx) override;
    Any visitTypeEnum(PostParser::TypeEnumContext* ctx) override;
    Any visitTypeTypedef(PostParser::TypeTypedefContext* ctx) override;
    Any visitFuncSpecifier(PostParser::FuncSpecifierContext* ctx) override;
    Any visitStorageSpecifier(PostParser::StorageSpecifierContext* ctx) override;
    Any visitDirId(PostParser::DirIdContext* ctx) override;
    Any visitPointer_item(PostParser::Pointer_itemContext* ctx) override;

    // Declarations
    Any visitDeclaration(PostParser::DeclarationContext* ctx) override;
    Any visitDeclarator(PostParser::DeclaratorContext* ctx) override;
    Any visitDirFunc(PostParser::DirFuncContext* ctx) override;
    Any visitFunc_def(PostParser::Func_defContext* ctx) override;
    Any visitParamDecl(PostParser::ParamDeclContext* ctx) override;
    Any visitParamAbst(PostParser::ParamAbstContext* ctx) override;

    // Expressions
    Any visitCall(PostParser::CallContext* ctx) override;


};

// class CompilerListener : PostBaseListener {

//   public:
//     CompilerListener() {}
//     ~CompilerListener() {}

//     void Process(ANTLRInputStream* stream);

//     void enterParse(PostParser::ParseContext* ctx) override;
//     void enterVariable_init(PostParser::Variable_initContext* ctx) override;

//   private:

//     std::vector<Variable> globals;
//     PostParser* parse;

// };