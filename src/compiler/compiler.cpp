
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
}

Any Compiler::visitTopDecl(PostParser::TopDeclContext* ctx)
{
  // TODO -- things like type specifiers need a simple key-value type 
  // so they can be added to the correct part of the declaration!!
  // (this can be a parse tree property)
  currentId = new Identifier;
  currentType = new Type;
  visitChildren(ctx);
  currentScope->AddSymbol(currentId->copy());
}

Any Compiler::visitTypeStd(PostParser::TypeStdContext* ctx)
{
  currentType->type_specifiers.push_back(ctx->getText());
  return nullptr;
}

Any Compiler::visitTypeStructUnion(PostParser::TypeStructUnionContext* ctx)
{
  // currentId.type = Identifier::IdType::
  visitChildren(ctx);
}

Any Compiler::visitTypeEnum(PostParser::TypeEnumContext* ctx)
{
  visitChildren(ctx);
}

Any Compiler::visitTypeTypedef(PostParser::TypeTypedefContext* ctx)
{
  currentType->name = ctx->IDENTIFIER()->getText();
  return nullptr;
}

Any Compiler::visitFuncSpecifier(PostParser::FuncSpecifierContext* ctx)
{
  currentType->function = Type::FunctionSpecifier::INLINE;
  return nullptr;
}

Any Compiler::visitStorageSpecifier(PostParser::StorageSpecifierContext* ctx)
{
  if (currentType->storageSet) {
    // TODO -- put error here
  }
  currentType->setStorage(ctx->getText());
}

Any Compiler::visitDirId(PostParser::DirIdContext* ctx)
{
  currentId->name = ctx->getText();
  return nullptr;
}

Any Compiler::visitPointer_item(PostParser::Pointer_itemContext* ctx)
{
  Pointer p;
  for (auto qual : ctx->type_qual())
  {
    p.setQualifier(qual->getText());
  }
  currentType->pointers.push_back(p);
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