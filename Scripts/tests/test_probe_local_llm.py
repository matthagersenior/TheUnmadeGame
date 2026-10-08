"""Offline tests for a loopback-only Ollama readiness probe."""
import io
import json
import sys
import unittest
from pathlib import Path

SCRIPTS = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(SCRIPTS))
from probe_local_llm import check_local_model, OllamaMissing, ModelMissing, BadResponse

class FakeResponse(io.BytesIO):
    pass

class OllamaReadinessTests(unittest.TestCase):
    def test_accepts_exact_local_model(self):
        def fake_open(request, timeout):
            self.assertEqual(request.full_url, "http://127.0.0.1:11434/api/tags")
            self.assertLessEqual(timeout, 5)
            return FakeResponse(json.dumps({"models": [{"name":"qwen3:4b-instruct"}]}).encode())
        self.assertEqual(check_local_model("qwen3:4b-instruct", opener=fake_open), "qwen3:4b-instruct")

    def test_missing_model_is_not_ready(self):
        def fake_open(request, timeout):
            return FakeResponse(json.dumps({"models": [{"name":"other:latest"}]}).encode())
        with self.assertRaises(ModelMissing):
            check_local_model("qwen3:4b-instruct", opener=fake_open)

    def test_bad_json_rejected(self):
        def fake_open(request, timeout):
            return FakeResponse(b"not-json")
        with self.assertRaises(BadResponse):
            check_local_model("qwen3:4b-instruct", opener=fake_open)

    def test_closed_local_server_gracefully_fails(self):
        from urllib.error import URLError
        def fake_open(request, timeout):
            raise URLError("connection refused")
        with self.assertRaises(OllamaMissing):
            check_local_model("qwen3:4b-instruct", opener=fake_open)

if __name__ == "__main__":
    unittest.main()
