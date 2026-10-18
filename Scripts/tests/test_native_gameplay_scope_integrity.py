"""Low-cost Unreal-source brace checks for every world, NPC and player gameplay unit.

The project runs portable C++ tests before the engine PC is available. This
scanner is not a substitute for UHT/Unreal compilation, but it prevents broken
function boundaries from silently passing a source-only CI suite.
"""
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[2]
GAME=ROOT/"Source/TheUnmadeGame/Private"

def braces(text):
    stack=[]
    i=0
    mode="code"
    while i<len(text):
        c=text[i];nxt=text[i+1] if i+1<len(text) else ""
        if mode=="code":
            if c=="/" and nxt=="/":mode="line";i+=2;continue
            if c=="/" and nxt=="*":mode="block";i+=2;continue
            if c=='"':mode="string";i+=1;continue
            if c=="'":mode="char";i+=1;continue
            if c=="{":stack.append(i)
            if c=="}":
                if not stack:return False,"unexpected closing brace"
                stack.pop()
        elif mode=="line":
            if c=="\n":mode="code"
        elif mode=="block":
            if c=="*" and nxt=="/":mode="code";i+=2;continue
        elif mode in ("string","char"):
            if c=="\\":i+=2;continue
            if c==('"' if mode=="string" else "'"):mode="code"
        i+=1
    if stack:
        line=text.count("\n",0,stack[0])+1
        return False,f"unclosed brace from line {line}"
    if mode in ("block","string","char"):return False,f"unterminated {mode}"
    return True,""

class NativeGameplayScopeIntegrity(unittest.TestCase):
 def test_world_npc_player_source_has_balanced_function_scopes(self):
  paths=sorted(p for folder in ("World","NPC","Player")
      for p in (GAME/folder).glob("*.cpp"))
  self.assertGreaterEqual(len(paths),20,"Expected the full nine-realm native source set")
  for path in paths:
   with self.subTest(path=str(path.relative_to(ROOT))):
    content=path.read_text(encoding="utf-8")
    valid,why=braces(content)
    self.assertTrue(valid,why)

 def test_brace_check_ignores_literals_and_comments_but_fails_nested_scopes(self):
  for sample in ('void f(){auto s="}"; /* { } */ if(true){}}',
                 "void f(){ // } brace in comment\nint x=1;}"):
   self.assertEqual(braces(sample),(True,""))
  self.assertFalse(braces('void f(){if(true){}')[0])
  self.assertFalse(braces('void f(){}}')[0])

if __name__=="__main__":unittest.main()
