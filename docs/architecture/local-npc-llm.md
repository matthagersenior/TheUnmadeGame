# THE UNMADE — Local NPC dialogue, first integration

**Status:** Source integration; Unreal compilation and live Ollama inference not yet verified.

## Policy
- All authoritative NPC facts, witnessed events, speaker provenance, trust, fear, quest rules and saves remain in Unreal C++.
- One shared `UGameInstanceSubsystem` submits an asynchronous dialogue-only request, never tools/function calls. It does not create game events, update memory, grant items, or alter quests.
- Model sees only one NPC's stable ID, display label, current trust/fear, and at most eight most-recent existing personal observations. Rumors are labeled with their speaker, not misrepresented as witnessed facts.
- The selected player's spoken line is bounded to 400 characters and treated as untrusted roleplay content, not world authority.
- Output must be `{"line":"..."}` from a completed Ollama /api/chat response, at most 240 characters and without control characters. Any failed request, malformed response, busy model, closed game, or disabled option falls back to deterministic `GetReactionText()`.
- HTTP is hardcoded to loopback `127.0.0.1:11434`. No external LLM endpoints, cloud fallbacks, remote scripts, paid APIs or telemetry. No network request is sent when the feature is disabled.
- This **cannot** perfectly eliminate fictional hallucinations in generated lines. The safety boundary is that *text cannot trigger gameplay actions*. Any future choices must go through verified game systems.

## Try locally on a Windows Unreal host
1. Install and run Ollama on the **same machine that runs the Unreal game**. Confirm your selected model's license. For example, use the Apache-2.0-licensed Qwen3 4B instruct variant:
   `ollama pull qwen3:4b-instruct`
2. Start Ollama normally; verify `http://127.0.0.1:11434/api/tags` responds locally. Do not expose Ollama to the internet.
3. Run `python Scripts/probe_local_llm.py --model qwen3:4b-instruct` to confirm Ollama is listening on loopback and the model has been downloaded. This doesn't run inference or require a cloud connection.\n4. In `Config/DefaultGame.ini` under `[/Script/TheUnmadeGame.UnmadeLocalDialogueSubsystem]`, set `bEnableLocalModel=true`. Default remains `false`.
5. Compile and run Unreal. Approach a resident, press **E** and read the generated line. If Ollama is missing, the model times out, the game is busy or output is invalid, the resident uses the existing deterministic response.
6. Run Unreal tests: `Scripts/run_ue_tests.ps1 -UnrealRoot <UnrealRoot> -TestFilter Unmade.LocalDialogue`.
7. Verify with an offline host (Ollama stopped) and a normal host. Log latency, GPU/RAM use and gameplay frame rate before enabling by default.

### Remote phone testing
When Pixel Streaming is eventually approved, the model must run on the **same remote Windows GPU host** as the Unreal game. The phone only receives streamed pixels, not model weights or a public Ollama endpoint.

### Packaging and cost
This is an **optional dev/runtime Ollama adapter**, *not* a bundled offline model distribution. Later, replace the transport through a shared local-model interface with a tested in-process llama.cpp backend for install-and-play PC shipping. That work requires license notice handling, model downloads or installer packaging, platform-specific library builds and RAM/VRAM profiling.

No paid AI service is necessary. Model weights are not checked into Git; current source does not download/install/run Ollama itself. No cloud GPU host is provisioned.

## Product policy update: AI is never required

The game must remain fully playable without the model. All NPC **gameplay decisions** now route through the separate native deterministic NPC policy, not through this subsystem. Local inference is strictly optional presentation of conversational text; no game world consequences, dialogue-choice availability, NPC disposition, inventory, quest progression, save format or trade permission may rely on a model response. The fallback line is authored in Unreal, and the optional LLM is disabled by default. See `docs/architecture/offline-npc-gameplay.md`.
