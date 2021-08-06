#!/usr/bin/python3

# import datetime
from argparse import ArgumentError
import os
import subprocess
from os.path import join
import re
import sys
import pickle
import shutil

"""
Use this manager to configure and build Corax projects.
"""

class Col:
    Stop="\033[0m";      # Text Reset

    # Regular Colors
    Black="\033[0;30m";       # Black
    Red="\033[0;31m";         # Red
    Redl="\033[0;91m";        # Light red
    Green="\033[0;32m";       # Green
    Yellow="\033[0;33m";      # Yellow
    Yellowl="\033[0;93m";     # Light yellow
    Blue="\033[0;34m";        # Blue
    Bluel="\033[0;94m";       # Light blue
    Purple="\033[0;35m";      # Purple
    Cyan="\033[0;36m";        # Cyan
    White="\033[0;37m";       # White
    Gray="\033[0;2m";

errors = {
    1: 'Input file could not be found.'
}

extension_strip = re.compile(r'^(.|\.)+(?=\.\S+$)')

class Warning:
    def __init__(self):
        self.warnings = []
        pass
    
    def add(self, message):
        self.warnings.append(message)
    
    def report(self):
        if len(self.warnings) > 0:
            for warn in self.warnings:
                print(Col.Bluel + 'Warning: ' + Col.Stop + warn)
            s = '' if len(self.warnings) == 1 else 's'
            print(f'\n{len(self.warnings)} warning{s} generated.')

warn = Warning()

def _queryUser(prompt, fallback='n'):
    if fallback.lower() in ('y', 'yes'):
        response = input(prompt + ' [Y/n] ')
        return response.lower().strip() not in ('n', 'no')
    elif fallback.lower() in ('n', 'no'):
        response = input(prompt + ' [y/N] ')
        return response.lower().strip() in ('y', 'yes')
    else:
        response = input(prompt + ' [y/N] ')
        while (response.lower().strip() not in ('y', 'yes', 'n', 'no')):
            response = input(prompt + Col.Redl + 'please provide a valid yes/no input ' + Col.Stop)
        return True if response.lower().strip() in ('y', 'yes') else False


def _error(message, error_code=1):
    print(Col.Redl + 'Error: ' + Col.Stop + message)
    sys.exit(error_code)

root = '.'
source = 'src'
src_dirs = [
  source, join(source, 'grammars'), 
  join(source, 'compiler'), 
  join(source, 'compiler', 'targets'),
  join(source, 'precompiler'),
]
build = 'build'
utils = join(root, source, 'util')
hashing = join(utils, 'hashing_parse')
antlr_jar = join(source, 'antlr4-jar')
antlr_jar = join(antlr_jar, os.listdir(antlr_jar)[0])

class ExecutionError(Exception):
  pass

def manageReturn(process, returnval, name: str, throw: bool=False):
  if returnval != 0:
        print(f'\nEncountered error while running {Col.Green}"{name}"{Col.Stop}:')
        print(process.stderr.read())
        process.stderr.close()
        if throw:
            raise ExecutionError
        print(f'({process.returncode}) exiting...')
        sys.exit(process.returncode)
  else:
    process.stderr.close()

# TODO -- add Popen based, asyncio subprocess call (https://stackoverflow.com/questions/16071866/non-blocking-subprocess-call)
# TODO -- the file hashers should be c++ and called by python. They should output to stdout and python can grab that after execution completes
def _execute(*args: str, cwd: str=None, throw: bool=False) -> None:
    proc = subprocess.run(args, cwd=cwd, capture_output=True)
    manageReturn(proc, args[0], throw)

# TODO -- it would still be nice to have a true async process so the caller could report progress on tasks other than cmake
def _execute_report(*args: str, cwd: str=None, throw: bool=False) -> None:
    proc = subprocess.Popen(args, cwd=cwd, stdout=subprocess.PIPE, universal_newlines=True, stderr=subprocess.PIPE)
    # for line in proc.stdout:
    #   print(line.decode(), end='')
    while True:
        nextline = proc.stdout.readline()
        if nextline == '' and proc.poll() is not None:
            break
        print(nextline, end='')
        # sys.stdout.write(nextline)
        # sys.stdout.flush()
    proc.stdout.close()
    returnval = proc.wait()
    manageReturn(proc, returnval, args[0], throw)

def _checkEnv(program):

  ready = True
   
  rd = os.path.abspath(__file__).replace(program, '').rstrip('/')
  if rd != os.path.abspath('.'):
    _error("script must be run from the root project directory.")

  if not os.path.exists(hashing) or len(os.listdir(hashing)) == 0:
    if not _queryUser('Hello! Looks like the build environment hasn\'t been set up yet. Begin setup?', fallback='y'):
      _error("build dependencies not present.")
    if not os.path.exists(hashing):
      os.mkdir(hashing)
    _execute(
      'java', '-jar', 
      join('../../', antlr_jar),
      '-no-listener',
      '-Dlanguage=Python3', '-o', 
      os.path.basename(hashing), 'AntlrHash.g4', 
      cwd=utils
    )
    _execute(
      'java', '-jar', 
      join('../../', antlr_jar),
      '-no-listener',
      '-Dlanguage=Python3', '-o', 
      os.path.basename(hashing), 'CppHash.g4', 
      cwd=utils
    )

    ready = False

  # note -- this should probably be replaced at some point with a setup.py
  try:
    import antlr4
  except ModuleNotFoundError:
    if not _queryUser('The Antlr 4 runtime was not found. Install with pip?', fallback='y'):
      print(f'Install manually by running "{sys.executable} -m pip install antlr4-python3-runtime"')
      _error("build dependencies not present.")
    try:
      _execute(sys.executable, '-m', 'pip', 'install', 'antlr4-python3-runtime', throw=True)
    except ExecutionError:
      _error(f'automatic pip install failed. Install manually by running "{sys.executable} -m pip install antlr4-python3-runtime"')

  if not ready:
    print("Build environment ready! You can now rerun the script.")
  
  return ready
    

  # if not 'gen' in os.listdir(assembler_dir):
  #     # TODO -- make this regenerate language files
  #     print('ouch owy my bones')
  #     exit(1)

def RunCompilation(update: bool = False, build_grammar: bool=False):
  if update:
    pass
  _execute_report(
    'make', 
    cwd=join(source, build)
  )

def RunProgram(**args):
  if not os.path.exists(join(source, build, 'corax')):
    _error('executable file not built.')

  out = []
  if '-f' in args:
    out.append(args['-f'])
  else:
    out.append(join(source, 'examples', 'test.cx'))
  for key, item in args.items():
    out.append(key)
    out.append(item)
  print(out)
  _execute_report(join(source, build, 'corax'), *out)

def ReportChanges(newf, changef, rmf):
  if len(newf) > 0:
    print('New files:')
    for f in newf:
      print('  ' + Col.Bluel + f + Col.Stop)
  if len(changef) > 0:
    print('Changes detected in the following files:')
    for f in changef:
      print('  ' + Col.Green + f + Col.Stop)
  if len(rmf) > 0:
    print('Removed files:')
    for f in rmf:
      print('  ' + Col.Gray + f + Col.Stop)

def FindChanges(*positional, **optional):

  commands = ('run', 'build')
  if positional[0] not in commands:
    _error(f'unkown command {positional[0]}')

  from src.util.hashing import HashManager
  try:
    with open(join(utils, 'manager.pkl'), 'rb') as file:
      manager = pickle.load(file)
  except FileNotFoundError:
    manager = HashManager(src_dirs)
  newf, changef, rmf = manager.check()

  if positional[0] == 'run':
    ReportChanges(newf, changef, rmf)
    if len(newf) > 0 or len(changef) > 0 or len(rmf) > 0:
      if _queryUser('Changes detected. Recompile?', fallback='y'):
        manager.save(join(utils, 'manager.pkl'))
        RunCompilation(update=True)
    RunProgram(**optional)

  if positional[0] == 'build':
    if len(newf) > 0 or len(changef) > 0 or len(rmf) > 0:
      update = True
    else:
      update = False
    RunCompilation(update=update)

  

if __name__ == '__main__':
    from argparse import ArgumentParser

    parser = ArgumentParser(
        description='Manage the project and build executable binaries.'
    )
    parser.add_argument(
        'command', metavar='command', type=str,
        help='command to be executed'
    )
    parser.add_argument(
        'name', metavar='input', type=str, nargs='?',
        help='optional test input'
    )

    if _checkEnv(parser.prog):
      args = parser.parse_args()

      positional = [args.command, args.name]
      optional = vars(args)
      del optional['name']
      del optional['command']

      FindChanges(*positional, **optional)