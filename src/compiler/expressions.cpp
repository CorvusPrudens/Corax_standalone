#include <iostream>
#include <string>
#include <stdio.h>

#include "compiler.h"
#include "operators.h"

using antlrcpp::Any;
using std::string;

////////////////////////////////////////////////
// EXPRESSIONS
////////////////////////////////////////////////

void Compiler::operation(antlr4::tree::ParseTree* ctx, Result op1, Result op2, OperatorBase& oper)
{
  Result res;

  if (op1.isConst() && op2.isConst()) {
    int priority1 = fetchPriority(op1.type);
    int priority2 = fetchPriority(op2.type);
    if (priority1 < priority2)
    {
      op2.to(op1.type);
    }
    else if (priority2 < priority1)
    {
      op1.to(op2.type);
    }

    Type type = op1.type;

    if (type == long_double_) {
      auto v1 = op1.as<long double>();
      auto v2 = op2.as<long double>();
      oper.perform(v1, v2, res);
    } else if (type == double_) {
      auto v1 = op1.as<double>();
      auto v2 = op2.as<double>();
      oper.perform(v1, v2, res);
    } else if (type == float_) {
      auto v1 = op1.as<float>();
      auto v2 = op2.as<float>();
      oper.perform(v1, v2, res);
    } else if (type == unsigned_long_long_) {
      auto v1 = op1.as<unsigned long long>();
      auto v2 = op2.as<unsigned long long>();
      oper.perform(v1, v2, res);
    } else if (type == long_long_) {
      auto v1 = op1.as<long long>();
      auto v2 = op2.as<long long>();
      oper.perform(v1, v2, res);
    } else if (type == unsigned_long_) {
      auto v1 = op1.as<unsigned long>();
      auto v2 = op2.as<unsigned long>();
      oper.perform(v1, v2, res);
    } else if (type == long_) {
      auto v1 = op1.as<long>();
      auto v2 = op2.as<long>();
      oper.perform(v1, v2, res);
    } else if (type == unsigned_) {
      auto v1 = op1.as<unsigned>();
      auto v2 = op2.as<unsigned>();
      oper.perform(v1, v2, res);
    } else if (type == int_) {
      auto v1 = op1.as<int>();
      auto v2 = op2.as<int>();
      oper.perform(v1, v2, res);
    } else if (type == unsigned_short_) {
      auto v1 = op1.as<unsigned short>();
      auto v2 = op2.as<unsigned short>();
      oper.perform(v1, v2, res);
    } else if (type == short_) {
      auto v1 = op1.as<short>();
      auto v2 = op2.as<short>();
      oper.perform(v1, v2, res);
    } else if (type == unsigned_char_) {
      auto v1 = op1.as<unsigned char>();
      auto v2 = op2.as<unsigned char>();
      oper.perform(v1, v2, res);
    } else if (type == signed_char_) {
      auto v1 = op1.as<signed char>();
      auto v2 = op2.as<signed char>();
      oper.perform(v1, v2, res);
    } else if (type == char_) {
      auto v1 = op1.as<char>();
      auto v2 = op2.as<char>();
      oper.perform(v1, v2, res);
    } else if (type == void_) {
      throw 1;
    } else {
      throw 2;
    }
  }
  else if (op1.isConst()) {
    oper.perform(op1, op2.id, res);
  } else if (op2.isConst()) {
    oper.perform(op1.id, op2, res);
  } else {
    oper.perform(op1.id, op2.id, res);
  }

  results.put(ctx, res);
}

void Compiler::operation(antlr4::tree::ParseTree* ctx, Result op1, OperatorBase& oper)
{
  Result res;

  if (op1.isConst()) {

    Type type = op1.type;

    if (type == long_double_) {
      auto v1 = op1.as<long double>();
      oper.perform(v1, res);
    } else if (type == double_) {
      auto v1 = op1.as<double>();
      oper.perform(v1, res);
    } else if (type == float_) {
      auto v1 = op1.as<float>();
      oper.perform(v1, res);
    } else if (type == unsigned_long_long_) {
      auto v1 = op1.as<unsigned long long>();
      oper.perform(v1, res);
    } else if (type == long_long_) {
      auto v1 = op1.as<long long>();
      oper.perform(v1, res);
    } else if (type == unsigned_long_) {
      auto v1 = op1.as<unsigned long>();
      oper.perform(v1, res);
    } else if (type == long_) {
      auto v1 = op1.as<long>();
      oper.perform(v1, res);
    } else if (type == unsigned_) {
      auto v1 = op1.as<unsigned>();
      oper.perform(v1, res);
    } else if (type == int_) {
      auto v1 = op1.as<int>();
      oper.perform(v1, res);
    } else if (type == unsigned_short_) {
      auto v1 = op1.as<unsigned short>();
      oper.perform(v1, res);
    } else if (type == short_) {
      auto v1 = op1.as<short>();
      oper.perform(v1, res);
    } else if (type == unsigned_char_) {
      auto v1 = op1.as<unsigned char>();
      oper.perform(v1, res);
    } else if (type == signed_char_) {
      auto v1 = op1.as<signed char>();
      oper.perform(v1, res);
    } else if (type == char_) {
      auto v1 = op1.as<char>();
      oper.perform(v1, res);
    } else if (type == void_) {
      throw 1;
    } else {
      throw 2;
    }
  } else {
    oper.perform(op1.id, res);
  }

  results.put(ctx, res);
}

Any Compiler::visitCall(PostParser::CallContext* ctx)
{
  if (graphing) graph.Addc(ctx->expr_primary()->getText());
  return visitChildren(ctx);
}

// Simple passthroughs
Any Compiler::visitExprCast(PostParser::ExprCastContext* ctx)
{
  visitChildren(ctx);
  Result res = results.get(ctx->children[0]);
  results.put(ctx, res);
  return nullptr;
}

Any Compiler::visitExprMulti(PostParser::ExprMultiContext* ctx)
{
  visitChildren(ctx);
  Result res = results.get(ctx->children[0]);
  results.put(ctx, res);
  return nullptr;
}

Any Compiler::visitExprTern(PostParser::ExprTernContext* ctx)
{
  visitChildren(ctx);
  Result res = results.get(ctx->children[0]);
  results.put(ctx, res);
  return nullptr;
}

Any Compiler::visitExprAssi(PostParser::ExprAssiContext* ctx)
{
  visitChildren(ctx);
  Result res = results.get(ctx->children[0]);
  results.put(ctx, res);
  return nullptr;
}

Any Compiler::visitExprList(PostParser::ExprListContext* ctx)
{
  visitChildren(ctx);
  Result res = results.get(ctx->children[0]);
  results.put(ctx, res);
  return nullptr;
}

Any Compiler::visitExprExpression(PostParser::ExprExpressionContext* ctx)
{
  visitChildren(ctx);
  Result res = results.get(ctx->children[0]);
  results.put(ctx, res);
  return nullptr;
}

// Simple primaries
Any Compiler::visitIdentifier(PostParser::IdentifierContext* ctx)
{
  Result res;
  try {
    auto id = currentScope->GetSymbol(ctx->getText());
    res.setValue(id);
  } catch (int e) {
    string errmess = "undefined identifier \"" + ctx->getText() + "\"";
    addRuleErr(ctx, errmess);
  }
  results.put(ctx, res);
  return nullptr;
}

Any Compiler::visitConstInt(PostParser::ConstIntContext* ctx)
{
  int value = stoi(ctx->getText());
  Result res;
  res.setValue(value);
  results.put(ctx, res);
  return nullptr;
}

Any Compiler::visitConstFlt(PostParser::ConstFltContext* ctx)
{
  float value = stof(ctx->getText());
  Result res;
  res.setValue(value);
  results.put(ctx, res);
  return nullptr;
}

// not sure what to do with a string yet
Any Compiler::visitString(PostParser::StringContext* ctx)
{
  Result res;
  results.put(ctx, res);
  return nullptr;
}

Any Compiler::visitGroup(PostParser::GroupContext* ctx)
{
  visitChildren(ctx);
  Result res = results.get(ctx->expression());
  results.put(ctx, res);
  return nullptr;
}

// simple arithmetic
Any Compiler::visitMult(PostParser::MultContext* ctx)
{
  visitChildren(ctx);
  Mult oper(ctx, currentScope, currentFunction);
  operation(ctx, results.get(ctx->expr_arith()[0]), results.get(ctx->expr_arith()[1]), oper);

  return nullptr;
}

// Assignments
Any Compiler::visitAssignment(PostParser::AssignmentContext* ctx)
{
  visitChildren(ctx);
  Assign oper(ctx, currentScope, currentFunction);
  operation(ctx, results.get(ctx->expr_primary()), results.get(ctx->expr_assi()), oper);

  return nullptr;
}

Any Compiler::visitDereference(PostParser::DereferenceContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitNegation(PostParser::NegationContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitIncrementUnary(PostParser::IncrementUnaryContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitAddress(PostParser::AddressContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitIndexing(PostParser::IndexingContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitIncrementPost(PostParser::IncrementPostContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitSizeof(PostParser::SizeofContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitPositive(PostParser::PositiveContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitDecrementPost(PostParser::DecrementPostContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitDecrementUnary(PostParser::DecrementUnaryContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitSizeofType(PostParser::SizeofTypeContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitIndirectMember(PostParser::IndirectMemberContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitNegative(PostParser::NegativeContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitNot(PostParser::NotContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitMember(PostParser::MemberContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitCast(PostParser::CastContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitMinus(PostParser::MinusContext *ctx) 
{
  visitChildren(ctx);
  Sub oper(ctx, currentScope, currentFunction);
  operation(ctx, results.get(ctx->expr_arith()[0]), results.get(ctx->expr_arith()[1]), oper);

  return nullptr;
}

Any Compiler::visitMod(PostParser::ModContext *ctx) 
{
  visitChildren(ctx);
  Mod oper(ctx, currentScope, currentFunction);
  try {
    operation(ctx, results.get(ctx->expr_arith()[0]), results.get(ctx->expr_arith()[1]), oper);
  } catch (int e) {
    string errmess = "modulo operator cannot accept operands of type \"";
    switch (e) {
      case 1:
        errmess += "long double";
        break;
      case 2:
        errmess += "double";
        break;
      default:
      case 3:
        errmess += "float";
        break;
    }
    addRuleErr(ctx, errmess + "\"");
    // we'll just put a const 0 for now in case of error
    Result res;
    res.setValue(0);
    results.put(ctx, res);
  }
  

  return nullptr;
}

Any Compiler::visitOr(PostParser::OrContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitNotEqual(PostParser::NotEqualContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitLess(PostParser::LessContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitBit_or(PostParser::Bit_orContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitPlus(PostParser::PlusContext *ctx) 
{
  visitChildren(ctx);
  Add oper(ctx, currentScope, currentFunction);
  operation(ctx, results.get(ctx->expr_arith()[0]), results.get(ctx->expr_arith()[1]), oper);

  return nullptr;
}

Any Compiler::visitGreater_equal(PostParser::Greater_equalContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitDiv(PostParser::DivContext *ctx) 
{
  visitChildren(ctx);
  Div oper(ctx, currentScope, currentFunction);
  operation(ctx, results.get(ctx->expr_arith()[0]), results.get(ctx->expr_arith()[1]), oper);

  return nullptr;
}

Any Compiler::visitEqual(PostParser::EqualContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitShiftLeft(PostParser::ShiftLeftContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitShiftRight(PostParser::ShiftRightContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitBit_xor(PostParser::Bit_xorContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitAnd(PostParser::AndContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitBit_and(PostParser::Bit_andContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitGreater(PostParser::GreaterContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitLess_equal(PostParser::Less_equalContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitTernary(PostParser::TernaryContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitAssignmentMult(PostParser::AssignmentMultContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitAssignmentDiv(PostParser::AssignmentDivContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitAssignmentMod(PostParser::AssignmentModContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitAssignmentPlus(PostParser::AssignmentPlusContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitAssignmentMinus(PostParser::AssignmentMinusContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitAssignmentShiftLeft(PostParser::AssignmentShiftLeftContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitAssignmentShiftRight(PostParser::AssignmentShiftRightContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitAssignmentBitAnd(PostParser::AssignmentBitAndContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitAssignmentBitXor(PostParser::AssignmentBitXorContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitAssignmentBitOr(PostParser::AssignmentBitOrContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitArglist(PostParser::ArglistContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitComma(PostParser::CommaContext *ctx) 
{
  return visitChildren(ctx);
}

Any Compiler::visitExpr_const(PostParser::Expr_constContext *ctx) 
{
  return visitChildren(ctx);
}
