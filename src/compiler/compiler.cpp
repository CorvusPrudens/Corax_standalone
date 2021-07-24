
#include <iostream>
#include <stdio.h>

#include "compiler.h"

using antlrcpp::Any;

void Compiler::Process(ProcessedCode* code_)
{
  code = code_;

  ANTLRInputStream stream(code->code);
  PostLexer lexer(&stream);
  CommonTokenStream tokens(&lexer);
  PostParser parser(&tokens);

  tree::ParseTree *tree = parser.parse();
  visit(tree);
}

Any Compiler::visitParse(PostParser::ParseContext* ctx)
{
  globalTable = new SymbolTable(nullptr, SymbolTable::Scope::GLOBAL);
  currentScope = globalTable;
  visitChildren(ctx);

  for (auto id : globalTable->symbols)
  {
    std::cout << id.name << "\n";
  }

  return nullptr;
}

Any Compiler::visitTopDecl(PostParser::TopDeclContext* ctx)
{
  // TODO -- things like type specifiers need a simple key-value type 
  // so they can be added to the correct part of the declaration!!
  // (this can be a parse tree property)
  auto tempid = new Identifier;
  currentId.push_back(tempid);
  visitChildren(ctx);
  currentScope->AddSymbol(tempid->copy());
  currentId.pop_back();

  return nullptr;
}

// ignoring declarations after parameter list for now
Any Compiler::visitTopFunc(PostParser::TopFuncContext* ctx)
{
  auto tempid = new Identifier;
  currentId.push_back(tempid);

  currentScope = new SymbolTable(currentScope, SymbolTable::Scope::FUNCTION);
  
  visit(ctx->func_def()->decl_spec());
  visit(ctx->func_def()->declarator());

  tempid->type = Identifier::IdType::FUNCTION;
  globalTable->AddSymbol(tempid->copy());
  currentId.pop_back();

  visit(ctx->func_def()->stat_compound());

  currentScope = currentScope->parent;

  return nullptr;
}

Any Compiler::visitParamList(PostParser::ParamListContext* ctx)
{
  auto list = ctx->param_list()->param_decl();
  for (auto item : list)
  {
    auto ti = new Identifier;
    currentId.push_back(ti);

    visit(item);

    currentScope->AddSymbol(ti->copy());
    currentId.pop_back();
  }
  return nullptr;
}

Any Compiler::visitBlockDecl(PostParser::BlockDeclContext* ctx)
{
  auto ti = new Identifier;
  currentId.push_back(ti);

  visitChildren(ctx);

  currentScope->AddSymbol(ti->copy());
  currentId.pop_back();
  return nullptr;
}

Any Compiler::visitStat_compound(PostParser::Stat_compoundContext* ctx)
{
  currentScope = new SymbolTable(currentScope, SymbolTable::Scope::LOCAL);
  visitChildren(ctx);
  currentScope = currentScope->parent;
  return nullptr;
}

// Any Compiler::visitBlockStat(PostParser::BlockStatContext* ctx)
// {
//   currentScope = new SymbolTable(currentScope, SymbolTable::Scope::LOCAL);
//   visitChildren(ctx);
//   currentScope = currentScope->parent;
// }

Any Compiler::visitTypeStd(PostParser::TypeStdContext* ctx)
{
  currentId.back()->dataType.type_specifiers.push_back(ctx->getText());
  return nullptr;
}

Any Compiler::visitTypeStructUnion(PostParser::TypeStructUnionContext* ctx)
{
  // currentId.type = Identifier::IdType::
  return visitChildren(ctx);
}

Any Compiler::visitTypeEnum(PostParser::TypeEnumContext* ctx)
{
  return visitChildren(ctx);
}

Any Compiler::visitTypeTypedef(PostParser::TypeTypedefContext* ctx)
{
  currentId.back()->dataType.name = ctx->IDENTIFIER()->getText();
  return nullptr;
}

Any Compiler::visitFuncSpecifier(PostParser::FuncSpecifierContext* ctx)
{
  currentId.back()->dataType.function = Type::FunctionSpecifier::INLINE;
  return nullptr;
}

Any Compiler::visitStorageSpecifier(PostParser::StorageSpecifierContext* ctx)
{
  if (currentId.back()->dataType.storageSet) {
    // TODO -- put error here
  }
  currentId.back()->dataType.setStorage(ctx->getText());

  return nullptr;
}

Any Compiler::visitDirId(PostParser::DirIdContext* ctx)
{
  currentId.back()->name = ctx->getText();
  return nullptr;
}

Any Compiler::visitPointer_item(PostParser::Pointer_itemContext* ctx)
{
  Pointer p;
  for (auto qual : ctx->type_qual())
  {
    try{
      p.setQualifier(qual->getText());
    }
    catch (int e)
    {
      // insert error here
    }
  }
  currentId.back()->dataType.pointers.push_back(p);

  return nullptr;
}

// void CompilerListener::Process(ANTLRInputStream* stream)
// {
//   PostLexer lexer(stream);
//   CommonTokenStream tokens(&lexer);
//   PostParser parser(&tokens);

//   parse = &parser;

//   tree::ParseTree *tree = parser.parse();
//   tree::ParseTreeWalker::DEFAULT.walk(this, tree);
// }

// void CompilerListener::enterParse(PostParser::ParseContext *ctx)
// {
  
// }

// void CompilerListener::enterVariable_init(PostParser::Variable_initContext *ctx)
// {
//   auto def = ctx->variable_def();
//   auto dec = ctx->variable_dec();

//   if (def != nullptr)
//   {
//     std::cout << def->type()->NAME()->getText() << "\n";
//     if (def->assignment() != nullptr)
//     {
//       std::cout << def->assignment()->variable()->NAME()->getText() << "\n";
//     }
//   }
    
//   if (dec != nullptr)
//   {
//     std::cout << dec->type()->NAME()->getText() << "\n";
//     std::cout << dec->NAME()->getText() << "\n";
//   }
    
// }