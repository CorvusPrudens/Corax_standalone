#pragma once

#ifndef COMPILER_H
#define COMPILER_H

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

// this needs to be moved to its own file
class OperatorBase {
  public:

    OperatorBase(antlr4::tree::ParseTree* c, SymbolTable* t, Identifier* f) {
      ctx = c;
      table = t;
      func = f;
    }

    virtual Instruction::Abstr getAbstr() { throw 1; } // this should always be overriden

    /////////////////////////////////////////
    // Binary expressions
    /////////////////////////////////////////

    virtual void perform(long double v1, long double v2, Result& res) {}
    virtual void perform(double v1, double v2, Result& res) {}
    virtual void perform(float v1, float v2, Result& res) {}
    virtual void perform(unsigned long long v1, unsigned long long v2, Result& res) {}
    virtual void perform(long long v1, long long v2, Result& res) {}
    virtual void perform(unsigned long v1, unsigned long v2, Result& res) {}
    virtual void perform(long v1, long v2, Result& res) {}
    virtual void perform(unsigned v1, unsigned v2, Result& res) {}
    virtual void perform(int v1, int v2, Result& res) {}
    virtual void perform(unsigned short v1, unsigned short v2, Result& res) {}
    virtual void perform(unsigned char v1, unsigned char v2, Result& res) {}
    virtual void perform(signed char v1, signed char v2, Result& res) {}
    virtual void perform(char v1, char v2, Result& res) {}

    // Currently, in an operation between an lvalue and constant, the
    // constant is always converted to the lvalue's type (which may
    // not always be desirable?)
    virtual void perform(Identifier& id, Result& op2, Result& res) {
      int priority1 = fetchPriority(id.dataType);
      int priority2 = fetchPriority(op2.type);

      Result lhs;
      lhs.setValue(id);

      string tempname = "temp_var_" + std::to_string(table->temp_vars++);
      Identifier ass;
      ass.name = tempname;

      if (priority1 == priority2) {
        ass.dataType = lhs.id.dataType;
        table->AddSymbol(ass);
        func->function.add(Instruction(ctx, getAbstr(), lhs, op2, ass));
      }
      else {
        op2.to(id.dataType);
        ass.dataType = lhs.id.dataType;
        table->AddSymbol(ass);
        func->function.add(Instruction(ctx, getAbstr(), lhs, op2, ass));
      }
      // else if (priority1 < priority2) {
      //   op2.to(id.dataType);
      //   ass.dataType = lhs.id.dataType;
      //   table->AddSymbol(ass);
      //   func->function.add(Instruction(ctx, getAbstr(), lhs, op2, ass));
      // }
      // else
      // {
      //   ass.dataType = op2.type;
      //   Result temp;
      //   temp.setValue(ass);

      //   func->function.add(Instruction(ctx, Instruction::Abstr::CONVERT, lhs, op2, ass));
      //   func->function.add(Instruction(ctx, getAbstr(), temp, op2, ass));
      // }
      res.setValue(ass);
    }
    virtual void perform(Result& op1, Identifier& id, Result& res) {
      int priority2 = fetchPriority(id.dataType);
      int priority1 = fetchPriority(op1.type);

      Result rhs;
      rhs.setValue(id);

      string tempname = "temp_var_" + std::to_string(table->temp_vars++);
      Identifier ass;
      ass.name = tempname;

      if (priority1 == priority2) {
        ass.dataType = op1.type;
        table->AddSymbol(ass);
        func->function.add(Instruction(ctx, getAbstr(), op1, rhs, ass));        
      }
      else {
        op1.to(id.dataType);
        ass.dataType = rhs.id.dataType;
        table->AddSymbol(ass);
        func->function.add(Instruction(ctx, getAbstr(), op1, rhs, ass));
      }
      // else if (priority1 < priority2) {

      //   ass.dataType = op1.type;
      //   Result temp;
      //   temp.setValue(ass);

      //   func->function.add(Instruction(ctx, Instruction::Abstr::CONVERT, rhs, op1, ass));
      //   func->function.add(Instruction(ctx, getAbstr(), op1, temp, ass));
      // }
      // else
      // {
      //   op1.to(id.dataType);
      //   ass.dataType = rhs.id.dataType;
      //   table->AddSymbol(ass);
      //   func->function.add(Instruction(ctx, getAbstr(), op1, rhs, ass));
      // }
      res.setValue(ass);
    }
    virtual void perform(Identifier& id1, Identifier& id2, Result& res) {
      int priority1 = fetchPriority(id1.dataType);
      int priority2 = fetchPriority(id2.dataType);

      Result lhs;
      lhs.setValue(id1);
      Result rhs;
      rhs.setValue(id2);

      string tempname = "temp_var_" + std::to_string(table->temp_vars++);
      Identifier ass;
      ass.name = tempname;

      if (priority1 == priority2)
      {
        ass.dataType = id1.dataType;
        table->AddSymbol(ass);
        func->function.add(Instruction(ctx, getAbstr(), lhs, rhs, ass));
      }
      else if (priority1 < priority2)
      {
        ass.dataType = id1.dataType;
        table->AddSymbol(ass);
        Result temp;
        temp.setValue(ass);

        func->function.add(Instruction(ctx, Instruction::Abstr::CONVERT, rhs, lhs, ass));
        func->function.add(Instruction(ctx, getAbstr(), lhs, temp, ass));
      }
      else if (priority1 > priority2)
      {
        ass.dataType = id2.dataType;
        table->AddSymbol(ass);
        Result temp;
        temp.setValue(ass);

        func->function.add(Instruction(ctx, Instruction::Abstr::CONVERT, lhs, rhs, ass));
        func->function.add(Instruction(ctx, getAbstr(), temp, rhs, ass));
      }
      res.setValue(ass);
    }

    /////////////////////////////////////////
    // Unary expressions
    /////////////////////////////////////////

    virtual void perform(long double v1, Result& res) {}
    virtual void perform(double v1, Result& res) {}
    virtual void perform(float v1, Result& res) {}
    virtual void perform(unsigned long long v1, Result& res) {}
    virtual void perform(long long v1, Result& res) {}
    virtual void perform(unsigned long v1, Result& res) {}
    virtual void perform(long v1, Result& res) {}
    virtual void perform(unsigned v1, Result& res) {}
    virtual void perform(int v1, Result& res) {}
    virtual void perform(unsigned short v1, Result& res) {}
    virtual void perform(unsigned char v1,  Result& res) {}
    virtual void perform(signed char v1, Result& res) {}
    virtual void perform(char v1, Result& res) {}

    virtual void perform(Identifier& id, Result& res) {

      Result lhs;
      lhs.setValue(id);

      string tempname = "temp_var_" + std::to_string(table->temp_vars++);
      Identifier ass;
      ass.name = tempname;

      ass.dataType = lhs.id.dataType;
      table->AddSymbol(ass);
      func->function.add(Instruction(ctx, getAbstr(), lhs, ass));

      res.setValue(ass);
    }

    // needs access to the symbols table!
    SymbolTable* table;
    // will add the instruction to the current function
    Identifier* func;
    // Needs the parse tree node
    antlr4::tree::ParseTree* ctx;
};

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
    Identifier* currentFunction;
    // TODO -- This should probably be refactored later to not suck
    std::vector<Identifier*> currentId;
    std::vector<Type*> currentType;

    Graph graph;
    bool graphing = false;
    bool inherit = false;
    bool func_decl_err;
    int unnamed_inc = 0;

    void addRuleErr(ParserRuleContext* rule, string errmess);
    void addRuleWarn(ParserRuleContext* rule, string warnmess);

    void operation(antlr4::tree::ParseTree* ctx, Result op1, Result op2, OperatorBase& oper);
    // unary
    void operation(antlr4::tree::ParseTree* ctx, Result op1, OperatorBase& oper);

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

#endif