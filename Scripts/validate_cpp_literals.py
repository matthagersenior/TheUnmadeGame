"""Catch lexical C++ generation defects without claiming Unreal compilation."""
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def errors(s):
 i=0;line=1;out=[]
 while i<len(s):
  if s.startswith('//',i):j=s.find('\n',i);i=len(s) if j<0 else j;continue
  if s.startswith('/*',i):j=s.find('*/',i+2);j=len(s) if j<0 else j+2;line+=s[i:j].count('\n');i=j;continue
  if s.startswith('R"',i):
   j=s.find('(',i+2);delimiter=s[i+2:j];end=s.find(')'+delimiter+'"',j+1);end=len(s) if end<0 else end+len(delimiter)+2;line+=s[i:end].count('\n');i=end;continue
  if s[i] in ('"',"'"):
   quote=s[i];i+=1
   while i<len(s):
    c=s[i];i+=1
    if c=='\\':
     if i<len(s) and s[i]=='\n':line+=1
     i+=1;continue
    if c=='\n':out.append(line);line+=1
    if c==quote:break
   continue
  if s[i]=='\\' and s[i:i+2]=='\\n':out.append(line)
  if s[i]=='\n':line+=1
  i+=1
 return out
def main():
 failures=[]
 for p in (ROOT/'Source').rglob('*'):
  if p.suffix in ('.cpp','.h'):
   for n in errors(p.read_text()):failures.append(f'{p.relative_to(ROOT)}:{n}')
 if failures:raise SystemExit('Invalid literal line breaks or escaped source newlines: '+', '.join(failures))
 print('PASS: all C++ source literals pass lexical checks; Unreal compilation is separate')
if __name__=='__main__':main()
