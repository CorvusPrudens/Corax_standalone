#include <iostream>
#include <string>
#include <stdio.h>

#include "compiler.h"

using antlrcpp::Any;
using std::string;

////////////////////////////////////////////////
// EXPRESSIONS
////////////////////////////////////////////////

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
    res.setValue(currentScope->GetSymbol(ctx->getText()));
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

  Result op1 = results.get(ctx->expr_arith()[0]);
  Result op2 = results.get(ctx->expr_arith()[1]);
  Result res;

  // TODO -- create object with some kind of mechanism (method with function pointer?)
  // that allows us to package all this up neatly instead of have 50 lines of code for
  // all 60 expressions
  if (op1.isConst() && op2.isConst()) {
    if (op1.kind == Result::Kind::VOID || op2.kind == Result::Kind::VOID)
    {
      
    }
    else if (op1.kind == Result::Kind::FLOAT || op2.kind == Result::Kind::FLOAT)
    {
      float v1 = op1.as<float>();
      float v2 = op2.as<float>();
      res.setValue(v1 * v2);
    }
    else if (op1.kind == Result::Kind::INT)
    {
      // string warnmess = "operating on signed and unsigned int";
      // addRuleWarn(ctx, warnmess);
      int v1 = op1.as<unsigned int>();
      int v2 = op2.as<unsigned int>();
      res.setValue(v1 * v2);
    }
    else if (op1.kind == Result::Kind::S_INT)
    {
      // string warnmess = "operating on signed and unsigned int";
      // addRuleWarn(ctx, warnmess);
      int v1 = op1.as<int>();
      int v2 = op2.as<int>();
      res.setValue(v1 * v2);
    }

    results.put(ctx, res);
    return nullptr;
  }


  string tempname = "temp_var_" + std::to_string(temp_vars++);
  Identifier temp;
  temp.name = tempname;
  currentScope->AddSymbol(temp);
  std::cout << tempname << " = "; // for testing

  if (!op1.isConst()) {
    if (op2.isConst()) {
      // obviously we'll need to change how we treat the const op and how the temp
      // variable is created / used
      std::cout << op1.value.id.name << "*" << op2.as<float>() << "\n";
    }
    else
    {
      std::cout << op1.value.id.name << "*" << op2.value.id.name << "\n";
    }
  }

  if (!op2.isConst()) {
    if (op1.isConst()) {
      // obviously we'll need to change how we treat the const op and how the temp
      // variable is created / used
      std::cout << op2.value.id.name << "*" << op1.as<float>() << "\n";
    }
    else
    {
      std::cout << op2.value.id.name << "*" << op1.value.id.name << "\n";
    }
  }

  // this method is less memory efficient, but it's quite easy!
  res.setValue(temp);
  results.put(ctx, res);
  return nullptr;
}

// Assignments
Any Compiler::visitAssignment(PostParser::AssignmentContext* ctx)
{
  visitChildren(ctx);

  Result op1 = results.get(ctx->expr_primary());
  Result op2 = results.get(ctx->expr_assi());
  Result res;

  if (op1.isConst()) {
    addRuleErr(ctx, "left hand side must be a modifiable lvalue");
    results.put(ctx, res);
    return nullptr;
  }

  std::cout << op1.value.id.name << " = "; // for testing

  if (op2.isConst()) {
    std::cout << op2.as<float>() << "\n";
  }
  else {
    std::cout << op2.value.id.name << "\n";
  }

  results.put(ctx, res); // what should be the result though??
  return nullptr;
}