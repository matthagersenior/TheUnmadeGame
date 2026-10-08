#!/usr/bin/env python3
"""Non-invasive check for a locally installed Ollama model. Never downloads or runs AI."""
from __future__ import annotations

import argparse
import json
import sys
from urllib.error import HTTPError, URLError
from urllib.request import Request, urlopen

LOOPBACK_TAGS = "http://127.0.0.1:11434/api/tags"

class OllamaMissing(Exception):
    pass

class ModelMissing(Exception):
    pass

class BadResponse(Exception):
    pass


def check_local_model(model: str, *, opener=urlopen) -> str:
    """Return model name if Ollama's localhost tag list contains it; otherwise fail closed."""
    req = Request(LOOPBACK_TAGS, headers={"Accept": "application/json"})
    try:
        with opener(req, timeout=4) as response:
            body = response.read(256_001)
    except (URLError, HTTPError, OSError) as exc:
        raise OllamaMissing("Local Ollama is not responding on loopback") from exc
    if len(body) > 256_000:
        raise BadResponse("Local Ollama tags list is too large")
    try:
        data = json.loads(body)
        models = data["models"]
        if not isinstance(models, list) or any(not isinstance(m, dict) for m in models):
            raise TypeError("models must be objects")
        names = [m.get("name") for m in models]
    except (ValueError, TypeError, KeyError) as exc:
        raise BadResponse("Unexpected Ollama response shape") from exc
    if model not in names:
        raise ModelMissing(f"Model '{model}' not installed locally")
    return model


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--model", default="qwen3:4b-instruct")
    args = parser.parse_args(argv)
    try:
        name = check_local_model(args.model)
    except (OllamaMissing, ModelMissing, BadResponse) as exc:
        print(f"NOT_READY: {exc}", file=sys.stderr)
        return 2
    print(f"READY: {name} is installed in local Ollama. Engine inference is not tested.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
