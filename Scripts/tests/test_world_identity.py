import unittest,tempfile,sys,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'Scripts'))
import build_world_identity as identity
import build_identity_audio as audio
import build_skeletal_performance_kit as rigs
class IdentityTests(unittest.TestCase):
 def test_generated_registry(self):self.assertEqual(identity.build(),(ROOT/"Source/TheUnmadeGame/Public/World/UnmadeIdentityRegistry.h").read_text())
 def test_actual_audio_bytes(self):
  with tempfile.TemporaryDirectory() as tmp:
   p=Path(tmp);audio.build(p);m=audio.validate(p);self.assertEqual(sum(a['loop'] for a in m['assets']),9)
   q=p/m['assets'][0]['file'];q.write_bytes(q.read_bytes()[:-4])
   with self.assertRaises(AssertionError):audio.validate(p)
 def test_motion_distinct_and_keyed(self):
  for kind in rigs.KINDS:
   d=rigs.make_actor(kind);self.assertEqual(len(d['animations']),19)
   self.assertEqual(len({json.dumps(a['samplers']) for a in d['animations']}),19)
 def test_world_state_atomic_and_optional(self):
  s=(ROOT/'Source/TheUnmadeGame/Private/World/UnmadeWorldIdentity.cpp').read_text();self.assertIn('Identity=before',s);self.assertIn('WriteWorldSnapshot(p)',s);self.assertLess(s.index('WriteWorldSnapshot(p)'),s.index('Heal(35)'))
  h=(ROOT/'Source/TheUnmadeGame/Private/World/UnmadePrototypeHub.cpp').read_text();self.assertIn('if(Save->bHasWorldIdentity)',h);self.assertIn('if(bIdentitySaveRejected)return false;',h)
