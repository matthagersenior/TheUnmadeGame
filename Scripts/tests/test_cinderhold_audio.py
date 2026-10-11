import struct,tempfile,unittest,wave
from pathlib import Path
from Scripts.build_cinderhold_audio import build,validate,RATE
class CinderholdAudio(unittest.TestCase):
    def test_signal_has_three_distinct_knocks_and_a_real_pause(self):
        with tempfile.TemporaryDirectory() as directory:
            m=build(directory);self.assertEqual(validate(directory),m)
            with wave.open(str(Path(directory)/'SFX_CH_cadence.wav'),'rb') as w:
                raw=w.readframes(w.getnframes())
            mono=struct.unpack('<'+'h'*(len(raw)//2),raw)[::2]
            def peak(start,end):return max(abs(x) for x in mono[int(start*RATE):int(end*RATE)])
            for start,end in ((.18,.31),(.52,.65),(1.85,2.25)):self.assertGreater(peak(start,end),1000)
            self.assertEqual(peak(.7,1.8),0)
            self.assertEqual(peak(0,.17),0)
            self.assertEqual(peak(2.3,2.8),0)
    def test_modified_source_audio_is_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            build(directory);p=Path(directory)/'SFX_CH_vent_cool.wav';p.write_bytes(p.read_bytes()+b'changed')
            with self.assertRaises(ValueError):validate(directory)
