# Binary asset admission

Install Git LFS before adding binaries: `git lfs install`. .gitattributes covers Unreal assets/maps, audio, raster art, binary models and video. Text JSON/glTF remains reviewable text.

Before the first binary admission, use a disposable branch to add a small licensed fixture covered by an LFS pattern. Confirm `git check-attr filter diff merge text -- <path>`, `git lfs ls-files`, and `git lfs fsck`. Push it and clone into a fresh directory; run `git lfs pull` and compare the original and downloaded SHA-256 bytes. Confirm the index contains an LFS pointer with `git show HEAD:<path>`.

Do not admit production binaries until upload and fresh-clone retrieval pass. Current rules are configured; remote transfer is not yet proven because this repository contains no tracked LFS binaries. Confirm storage/bandwidth availability before large submissions.

CI runs Scripts/validate_hygiene.py against tracked index bytes, rejecting raw binaries or malformed LFS pointers. This verifies admission format independently of whether checkout downloaded the asset. It does not prove remote LFS upload/retrieval.
