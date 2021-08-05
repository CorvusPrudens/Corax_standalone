from genericpath import getmtime
import hashlib
from math import fabs
import os
from os.path import join
import re
import pickle
from typing import List

from antlr4 import *
from antlr4.tree.Tree import ParseTree
from .hashing_parse.CppHashLexer import CppHashLexer
from .hashing_parse.CppHashParser import CppHashParser
from .hashing_parse.CppHashListener import CppHashListener

from .hashing_parse.AntlrHashLexer import AntlrHashLexer
from .hashing_parse.AntlrHashParser import AntlrHashParser
from .hashing_parse.AntlrHashListener import AntlrHashListener

"""
Module for easy-to-use class for file comparisons and storing.
"""

BUF_SIZE = 65536

re_oneline = re.compile(r'//.+\n')
re_multi = re.compile(r'/\*[\s\S]*?\*/')
# don't remove first char of these:
re_empty = re.compile(r'\n\s*?\n')
re_spurious = re.compile(r'. +\n')
re_leading = re.compile(r'\n\s+?\S')

re_extra = re.compile(r'[ \t]{2,}')

re_single = re.compile(r'([^=!><\-+]|^)(=)([^=]|$)')

re_include = re.compile(r'`include *"(.+)"')
# parameter ramSize_p  = "heres/a/file"
re_important_file_1 = re.compile(r'parameter +[A-Za-z_]+ *= *"(.+?)"')
# .initFile_p("../units/include/promdata.hex")
re_important_file_2 = re.compile(r'\.+[A-Za-z_]+ *\( *"(.+?)" *\)')

re_file = re.compile(r'^((.|..|/|//|.?[\w-]+)?(/[\w-]+)+(\.[\w-]+)?)$')

# Calculates a hash as it listens
class CppListener(CppHashListener):

  def __init__(self, filepath: str, debug: bool=False):
    self.md5 = hashlib.md5()
    self.debug = debug

    input_stream = FileStream(filepath)
    lexer = CppHashLexer(input_stream)
    stream = CommonTokenStream(lexer)
    parser = CppHashParser(stream)
    tree = parser.parse()
    walker = ParseTreeWalker()
    walker.walk(self, tree)

  def __str__(self):
    return self.md5.hexdigest()

  def enterEveryRule(self, ctx: ParserRuleContext):
      self.md5.update(ctx.getText().encode('utf-8'))
      if self.debug:
        print(ctx.getText())

class AntlrListener(AntlrHashListener):

  def __init__(self, filepath: str, debug: bool=False):
    self.md5 = hashlib.md5()
    self.debug = debug

    input_stream = FileStream(filepath)
    lexer = AntlrHashLexer(input_stream)
    stream = CommonTokenStream(lexer)
    parser = AntlrHashParser(stream)
    tree = parser.parse()
    walker = ParseTreeWalker()
    walker.walk(self, tree)

  def __str__(self):
    return self.md5.hexdigest()

  def enterEveryRule(self, ctx: ParserRuleContext):
      self.md5.update(ctx.getText().encode('utf-8'))
      if self.debug:
        print(ctx.getText())

class CodeHash:

  def __init__(self, filepath: str):
    self.path = filepath
    self.value = self.getHash()
    self.temp = self.value

  def getHash(self) -> str:
    if (self.path[-3:] == '.g4'):
      hasher = AntlrListener(self.path)
    else:
      hasher = CppListener(self.path)
    return str(hasher)

  def isChanged(self) -> bool:
    # try:
    self.temp = self.getHash()
    if self.temp != self.value:
      return True
    return False
    # actually, a filenotfounderror should fall through
    # except FileNotFoundError:
    #   return True

  def writeChanges(self):
    self.value = self.temp

class HashManager:

  def __init__(self, src_dirs: 'list[str]', file_exts: 'list[str]'=['.cpp', '.h', '.g4']):
    self.src = src_dirs
    self.exts = file_exts
    self.hashes = {}
    self.new = []
    self.changed = []
    self.removed = []

  def isCode(self, path: str) -> bool:
    for ext in self.exts:
      if path[-len(ext):] == ext:
        return True
    return False

  def check(self):
    self.new = []
    self.changed = []
    self.removed = []
    for dir in self.src:
      for file in os.listdir(dir):
        full = join(dir, file)
        if not os.path.isdir(full) and self.isCode(file):
          if full not in self.hashes:
            self.new.append(full)
            continue
          try:
            changed = self.hashes[full].isChanged()
            if changed:
              self.changed.append(full)
          except FileNotFoundError:
            self.removed.append(full)
    return self.new, self.changed, self.removed

  def save(self, path: str):
    for f in self.new:
      self.hashes[f] = CodeHash(f)
    self.new = []
    for f in self.changed:
      self.hashes[f].wwriteChanges()
    for f in self.removed:
      del self.hashes[f]
    with open(path, 'wb') as file:
      pickle.dump(self, file)




def _generateHash(filepath: str) -> 'tuple[str, int]':
    # this bad boy oughta be enough for my purposes
    file_size = os.path.getsize(filepath)
    md5 = hashlib.md5()
    with open(filepath, 'rb') as file:
        while True:
            data = file.read(BUF_SIZE)
            if not data:
                break
            md5.update(data)
    return md5.hexdigest(), file_size


def _generateHashStr(string: str) -> str:
    md5 = hashlib.md5(bytes(string, 'utf-8'))
    return md5.hexdigest()


def _sar(string: str, regex: re.Match, replace: str='', group: int=0, ignore_first: bool=False, ignore_last: bool=False) -> str:
    """
    Search and replace using regex. Can optionally keep the first 
    or last character in the match, or only replace a group within a match.
    """
    offset = 1 if ignore_first else 0
    endoff = 1 if ignore_last else 0
    match = regex.search(string)
    while (match is not None):
        string = string[:match.start(group) + offset] + replace + string[match.end(group) - endoff:]
        match = regex.search(string, pos=match.start(group) + offset + len(replace))
    
    return string.strip(' \n')


def _spacePunc(string: str) -> str:
    punc = (',', '(', ')', '[', ']', ':', '+', '-', '/', '*', ';', '==', '<=', '>=', '!=')
    for p in punc:
        string = string.replace(p, f' {p} ')
    return _sar(string, re_single, replace=' = ', group=2)

class ArchitectureExistsError(Exception):
    pass

class Architecture:

    """
    A class that saves hashes of the source code and
    generated binary to keep up to date with changes.
    Makes a good effort to locate every file, but
    may not get every single one.
    """

    def __init__(self, top_filepath: str, build_dir: str, arch_name: str='default'):
        # sanitized, size = self.sanitizeVerilog(top_filepath)
        # print(sanitized)
        # print(_generateHashStr(sanitized))
        self.files = {}
        self.recursiveAdd(top_filepath, '.')
        self.arch_name = arch_name
        self.build_name = os.path.join(build_dir, arch_name)


    def recursiveAdd(self, filepath: str, current_dir: str):
        filepath = os.path.join(current_dir, filepath)
        with open(filepath, 'r') as file:
            string = file.read()
        
        imports, hex1, hex2 = self.getImports(string)

        for file in imports:
            self.recursiveAdd(file, os.path.dirname(filepath))
        
        # this could be fixed with proper search paths
        try:
            for file in hex1:
                fulldir = os.path.join(current_dir, file)
                self.files[fulldir] = Hex(fulldir)
            for file in hex2:
                fulldir = os.path.join(current_dir, file)
                self.files[fulldir] = Hex(fulldir)
        except FileNotFoundError:
            pass

        self.files[filepath] = Verilog(filepath)


    def getImports(self, string: str) -> 'tuple[list[str]]':
        imports = [s[1] for s in re_include.finditer(string)]
        hex1 = [s[1] for s in re_important_file_1.finditer(string)]
        hex2 = [s[1] for s in re_important_file_2.finditer(string)]
        return imports, hex1, hex2
    
    def save(self, buildpath: str, force: bool = False):
        name = self.getSaveName()
        if os.path.exists(name) and not force:
            raise ArchitectureExistsError(f'"{self.arch_name}" already exists!')
        with open(name, 'wb') as file:
            pickle.dump(self, file)
        
    def getSaveName(self) -> str:
        return self.build_name + '.pkl'
    
    def getBinName(self) -> str:
        return self.build_name + 'bin'
    
    def exists(self) -> bool:
        name = self.getSaveName()
        return os.path.exists(name)


    
    def compare(self, other: 'Architecture') -> bool:
        
        for key, item in self.files.items():
            try:
                if not item.compare(other.files[key]):
                    return False
            except KeyError:
                return False
        
        return True



class Verilog:

    def __init__(self, filepath: str):
        sanitized, size = self.sanitizeVerilog(filepath)
        hash = _generateHashStr(sanitized)
        self.name = os.path.abspath(filepath) # not gonna really work without some work?
        self.hash = hash
        self.size = size

    def compare(self, other: 'Verilog') -> bool:
        # actually, pre-sanitized size will always
        # make my testig fail haha
        # if self.size != other.size:
        #     return False
        if self.hash != other.hash:
            return False
        return True

    def sanitizeVerilog(self, filepath: str) -> 'tuple[str, int]':

        """
        Removes ~some~ changes that don't affect code, but isn't immune
        to all unimportant changes.
        """
        # ^ just say "changes detected, would you like to regenerate?"

        size = os.path.getsize(filepath)

        with open(filepath, 'r') as file:
            string = file.read()

        string = string.replace('\n', ' ')
        string = _sar(string, re_oneline)
        string = _sar(string, re_multi)
        string = _sar(string, re_empty, ignore_first=True)
        string = _spacePunc(string)
        string = _sar(string, re_spurious, ignore_first=True)
        string = _sar(string, re_leading, ignore_first=True, ignore_last=True)
        string = _sar(string, re_extra, ' ')

        return string, size
    
class Hex:

    def __init__(self, filepath: str):
        hash, size = _generateHash(filepath)
        self.name = os.path.abspath(filepath) # not gonna really work without some work?
        self.hash = hash
        self.size = size

    def compare(self, other: 'Verilog') -> bool:
        if self.size != other.size:
            return False
        if self.hash != other.hash:
            return False
        return True