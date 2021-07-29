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
    tree::ParseTreeProperty<Result> results;

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
    int temp_vars = 0;

    void addRuleErr(ParserRuleContext* rule, string errmess);
    void addRuleWarn(ParserRuleContext* rule, string warnmess);

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
    Any visitDereference(PostParser::DereferenceContext *ctx) override;
    Any visitIdentifier(PostParser::IdentifierContext *ctx) override;
    Any visitNegation(PostParser::NegationContext *ctx) override;
    Any visitIncrementUnary(PostParser::IncrementUnaryContext *ctx) override;
    Any visitAddress(PostParser::AddressContext *ctx) override;
    Any visitConstInt(PostParser::ConstIntContext *ctx) override;
    Any visitConstFlt(PostParser::ConstFltContext *ctx) override;
    Any visitString(PostParser::StringContext *ctx) override;
    Any visitIndexing(PostParser::IndexingContext *ctx) override;
    Any visitIncrementPost(PostParser::IncrementPostContext *ctx) override;
    Any visitSizeof(PostParser::SizeofContext *ctx) override;
    Any visitPositive(PostParser::PositiveContext *ctx) override;
    Any visitDecrementPost(PostParser::DecrementPostContext *ctx) override;
    Any visitDecrementUnary(PostParser::DecrementUnaryContext *ctx) override;
    Any visitCall(PostParser::CallContext *ctx) override;
    Any visitSizeofType(PostParser::SizeofTypeContext *ctx) override;
    Any visitIndirectMember(PostParser::IndirectMemberContext *ctx) override;
    Any visitNegative(PostParser::NegativeContext *ctx) override;
    Any visitNot(PostParser::NotContext *ctx) override;
    Any visitMember(PostParser::MemberContext *ctx) override;
    Any visitGroup(PostParser::GroupContext *ctx) override;
    Any visitExprCast(PostParser::ExprCastContext *ctx) override;
    Any visitCast(PostParser::CastContext *ctx) override;
    Any visitMinus(PostParser::MinusContext *ctx) override;
    Any visitMult(PostParser::MultContext *ctx) override;
    Any visitMod(PostParser::ModContext *ctx) override;
    Any visitOr(PostParser::OrContext *ctx) override;
    Any visitExprMulti(PostParser::ExprMultiContext *ctx) override;
    Any visitNotEqual(PostParser::NotEqualContext *ctx) override;
    Any visitLess(PostParser::LessContext *ctx) override;
    Any visitBit_or(PostParser::Bit_orContext *ctx) override;
    Any visitPlus(PostParser::PlusContext *ctx) override;
    Any visitGreater_equal(PostParser::Greater_equalContext *ctx) override;
    Any visitDiv(PostParser::DivContext *ctx) override;
    Any visitEqual(PostParser::EqualContext *ctx) override;
    Any visitShiftLeft(PostParser::ShiftLeftContext *ctx) override;
    Any visitShiftRight(PostParser::ShiftRightContext *ctx) override;
    Any visitBit_xor(PostParser::Bit_xorContext *ctx) override;
    Any visitAnd(PostParser::AndContext *ctx) override;
    Any visitBit_and(PostParser::Bit_andContext *ctx) override;
    Any visitGreater(PostParser::GreaterContext *ctx) override;
    Any visitLess_equal(PostParser::Less_equalContext *ctx) override;
    Any visitExprTern(PostParser::ExprTernContext *ctx) override;
    Any visitTernary(PostParser::TernaryContext *ctx) override;
    Any visitExprAssi(PostParser::ExprAssiContext *ctx) override;
    Any visitAssignment(PostParser::AssignmentContext *ctx) override;
    Any visitAssignmentMult(PostParser::AssignmentMultContext *ctx) override;
    Any visitAssignmentDiv(PostParser::AssignmentDivContext *ctx) override;
    Any visitAssignmentMod(PostParser::AssignmentModContext *ctx) override;
    Any visitAssignmentPlus(PostParser::AssignmentPlusContext *ctx) override;
    Any visitAssignmentMinus(PostParser::AssignmentMinusContext *ctx) override;
    Any visitAssignmentShiftLeft(PostParser::AssignmentShiftLeftContext *ctx) override;
    Any visitAssignmentShiftRight(PostParser::AssignmentShiftRightContext *ctx) override;
    Any visitAssignmentBitAnd(PostParser::AssignmentBitAndContext *ctx) override;
    Any visitAssignmentBitXor(PostParser::AssignmentBitXorContext *ctx) override;
    Any visitAssignmentBitOr(PostParser::AssignmentBitOrContext *ctx) override;
    Any visitArglist(PostParser::ArglistContext *ctx) override;
    Any visitExprList(PostParser::ExprListContext *ctx) override;
    Any visitComma(PostParser::CommaContext *ctx) override;
    Any visitExprExpression(PostParser::ExprExpressionContext *ctx) override;
    Any visitExpr_const(PostParser::Expr_constContext *ctx) override;

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