
#include <iostream>
#include <stdio.h>

#include "compiler.h"

using antlrcpp::Any;

void Compiler::Process(ProcessedCode* code_, Error* err_)
{
  code = code_;
  err = err_;

  ANTLRInputStream stream(code->code);
  PostLexer lexer(&stream);
  CommonTokenStream tokens(&lexer);
  PostParser parser(&tokens);

  tree::ParseTree *tree = parser.parse();
  visit(tree);

  if (graphing) graph.Print();
  else err->Report();
}

void Compiler::addRuleErr(ParserRuleContext* rule, string errmess)
{
  int outline = rule->start->getLine() - 1; // zero-indexed
  int cols = rule->start->getCharPositionInLine();
  int cole = cols + rule->stop->getStopIndex() - rule->start->getStartIndex();
  int line = code->lines.getLine(outline);
  string file = code->lines.getFile(outline);
  err->AddError(errmess, line, file, 1, cols, cole);
}

void Compiler::addRuleWarn(ParserRuleContext* rule, string warnmess)
{
  int outline = rule->start->getLine() - 1; // zero-indexed
  int cols = rule->start->getCharPositionInLine();
  int cole = cols + rule->stop->getStopIndex() - rule->start->getStartIndex();
  int line = code->lines.getLine(outline);
  string file = code->lines.getFile(outline);
  err->AddWarning(warnmess, line, file, 1, cols, cole);
}

Any Compiler::visitParse(PostParser::ParseContext* ctx)
{
  globalTable = new SymbolTable(nullptr, SymbolTable::Scope::GLOBAL);
  currentScope = globalTable;
  visitChildren(ctx);

  // for (auto id : globalTable->symbols)
  // {
  //   std::cout << id.name << "\n";
  //   if (id.members.size() > 0)
  //     for (auto mem : id.members)
  //       std::cout << "- " << mem.name << "\n";
  // }

  return nullptr;
}

////////////////////////////////////////////////
// DECLARATIONS
////////////////////////////////////////////////

// Any Compiler::visitTopDecl(PostParser::TopDeclContext* ctx)
// {
//   auto tempid = new Identifier;
//   currentId.push_back(tempid);
//   visitChildren(ctx);
//   try {
//     currentScope->AddSymbol(tempid->copy());
//   } catch (int e) {
//     string errmess = "redefinition of identifier ";
//     addRuleErr(ctx, errmess + "\"" + tempid->name + "\"");
//   }
  
//   currentId.pop_back();

//   return nullptr;
// }

Any Compiler::visitDeclaration(PostParser::DeclarationContext* ctx)
{
  auto temptype = new Type;
  currentType.push_back(temptype);
  visitChildren(ctx);
  currentType.pop_back();
  return nullptr;
}

Any Compiler::visitDeclarator(PostParser::DeclaratorContext* ctx)
{
  auto tempid = new Identifier;
  tempid->dataType = *currentType.back(); // if we're in an init_decl list
  currentId.push_back(tempid);
  visitChildren(ctx); // this adds pointers too!
  try {
    currentScope->AddSymbol(tempid->copy());
  } catch (int e) {
    string errmess;
    switch (e)
    {
      default:
      case 1:
        errmess = "redefinition of identifier ";
        break;
      case 2:
        errmess = "redefinition of function ";
        func_decl_err = true;
        break;
      case 3:
        errmess = "function definition does not match prototype ";
        func_decl_err = true;
        break;
    }
    addRuleErr(ctx, errmess + "\"" + tempid->name + "\"");
  }
  currentId.pop_back();
  return nullptr;
}

Any Compiler::visitDirFunc(PostParser::DirFuncContext* ctx)
{
  visit(ctx->direct_decl());
  currentScope = new SymbolTable(currentScope, SymbolTable::Scope::FUNCTION);
  if (ctx->param_type_list() != nullptr)
    visit(ctx->param_type_list());

  for (auto arg : currentScope->symbols)
    currentId.back()->members.push_back(arg);

  currentScope = currentScope->parent;
  // visit(ctx->direct_decl());
  // if (ctx->param_type_list() != nullptr)
  //   visit(ctx->param_type_list());

  if (graphing) graph.Addf(currentId.back()->name);

  currentId.back()->type = Identifier::IdType::FUNCTION;

  return nullptr;
}

Any Compiler::visitParamDecl(PostParser::ParamDeclContext* ctx)
{
  auto tt = new Type;
  currentType.push_back(tt);
  visitChildren(ctx);
  currentType.pop_back();

  return nullptr;
}

Any Compiler::visitParamAbst(PostParser::ParamAbstContext* ctx)
{
  auto tt = new Type;
  currentType.push_back(tt);
  if (ctx->abstract_decl() == nullptr)
  {
    visit(ctx->decl_spec());
    auto ti = new Identifier;
    ti->dataType = *tt;
    string name = "unnamed_" + std::to_string(unnamed_inc++);
    ti->name = name;
    // no need to catch error since this will never collide
    currentScope->AddSymbol(ti->copy());
  }
  else
    visitChildren(ctx);
  currentType.pop_back();

  return nullptr;
}

// ignoring declarations after parameter list for now
Any Compiler::visitFunc_def(PostParser::Func_defContext* ctx)
{
  // auto tempid = new Identifier;
  // currentId.push_back(tempid);
  auto temptype = new Type;
  currentType.push_back(temptype);

  // Maybe a bit hacky, but we'll skip compiling the function if its definition had an error
  func_decl_err = false;

  // currentScope = new SymbolTable(currentScope, SymbolTable::Scope::FUNCTION);
  
  visit(ctx->decl_spec());
  visit(ctx->declarator());

  // if (graphing) graph.Addf(tempid->name);

  // tempid->type = Identifier::IdType::FUNCTION; // note -- already happens in declarator()
  // try {
  //   globalTable->AddSymbol(tempid->copy());
  // } catch (int e) {
  //   string errmess;
  //   switch (e)
  //   {
  //     default:
  //     case 2:
  //       errmess = "redefinition of function ";
  //       break;
  //     case 3:
  //       errmess = "function definition does not match prototype ";
  //       break;
  //   }
  //   addRuleErr(ctx->declarator(), errmess + "\"" + tempid->name + "\"");
  //   currentId.pop_back();
  //   currentScope = currentScope->parent;
  //   return nullptr;
  // }
  // currentId.pop_back();
  currentType.pop_back();

  if (!func_decl_err)
  {
    // construct new scope from args
    currentScope = new SymbolTable(currentScope, SymbolTable::Scope::FUNCTION);
    inherit = true;
    // the last identifier added to the parent scope is the function
    currentFunction = &(*currentScope->parent->symbols.end()); // NOTE -- dangerous, this vector CANNOT be modified while using this pointer
    for (auto arg : currentScope->parent->symbols.back().members)
      currentScope->AddSymbol(arg);
    visit(ctx->stat_compound());
    currentScope = currentScope->parent;
  }

  return nullptr;
}

// // ignoring declarations after parameter list for now
// Any Compiler::visitTopFunc(PostParser::TopFuncContext* ctx)
// {
//   auto tempid = new Identifier;
//   currentId.push_back(tempid);

//   currentScope = new SymbolTable(currentScope, SymbolTable::Scope::FUNCTION);
  
//   visit(ctx->func_def()->decl_spec());
//   visit(ctx->func_def()->declarator());

//   if (graphing) graph.Addf(tempid->name);

//   tempid->type = Identifier::IdType::FUNCTION;
//   try {
//     globalTable->AddSymbol(tempid->copy());
//   } catch (int e) {
//     string errmess;
//     switch (e)
//     {
//       default:
//       case 2:
//         errmess = "redefinition of function ";
//         break;
//       case 3:
//         errmess = "function definition does not match prototype ";
//         break;
//     }
//     addRuleErr(ctx->func_def()->declarator(), errmess + "\"" + tempid->name + "\"");
//     currentId.pop_back();
//     currentScope = currentScope->parent;
//     return nullptr;
//   }
//   currentId.pop_back();

//   visit(ctx->func_def()->stat_compound());

//   currentScope = currentScope->parent;

//   return nullptr;
// }

Any Compiler::visitParamList(PostParser::ParamListContext* ctx)
{
  auto list = ctx->param_list()->param_decl();
  for (auto item : list)
  {
    auto ti = new Identifier;
    currentId.push_back(ti);

    visit(item);

    // try {
    //   currentScope->AddSymbol(ti->copy());
    // } catch (int e) {
    //   string errmess;
    //   switch (e)
    //   {
    //     default:
    //     case 1:
    //       errmess = "redefinition of identifier ";
    //       break;
    //     case 2:
    //       errmess = "redefinition of function ";
    //       break;
    //     case 3:
    //       errmess = "function definition does not match prototype ";
    //       break;
    //   }
    //   addRuleErr(item, errmess + "\"" + ti->name + "\"");
    // }
    // currentType.pop_back();
    currentId.pop_back();
  }
  return nullptr;
}

// Any Compiler::visitBlockDecl(PostParser::BlockDeclContext* ctx)
// {
//   auto ti = new Identifier;
//   currentId.push_back(ti);

//   visitChildren(ctx);

//   try {
//     currentScope->AddSymbol(ti->copy());
//   } catch (int e) {
//     string errmess = "redefinition of identifier ";
//     addRuleErr(ctx, errmess + "\"" + ti->name + "\"");
//   }

//   currentId.pop_back();
//   return nullptr;
// }

Any Compiler::visitStat_compound(PostParser::Stat_compoundContext* ctx)
{
  if (inherit)
  {
    inherit = false;
    visitChildren(ctx);
  }
  else
  {
    currentScope = new SymbolTable(currentScope, SymbolTable::Scope::LOCAL);
    visitChildren(ctx);
    currentScope = currentScope->parent;
  }
  
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
  currentType.back()->type_specifiers.push_back(ctx->getText());
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
  currentType.back()->name = ctx->IDENTIFIER()->getText();
  return nullptr;
}

Any Compiler::visitFuncSpecifier(PostParser::FuncSpecifierContext* ctx)
{
  currentType.back()->function = Type::FunctionSpecifier::INLINE;
  return nullptr;
}

Any Compiler::visitStorageSpecifier(PostParser::StorageSpecifierContext* ctx)
{
  if (currentType.back()->storageSet) {
    string errmess = "identifier cannot have more than one storage class";
    addRuleErr(ctx, errmess);
  }
  else
    currentType.back()->setStorage(ctx->getText());

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
      // NOTE -- actually this can't occur, there would be a syntax error (right?)
      string errmess = "unexpected type qualifier " + qual->getText();
      addRuleErr(qual, errmess);
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