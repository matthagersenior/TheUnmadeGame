# Unreal runner acceptance

The Unreal workflow is manual until an engine host exists. Provision a dedicated trusted Windows x64 self-hosted runner labeled `unreal`, with Git LFS, Python 3.11, compatible Visual Studio C++ tooling and Unreal 5.8. Set repository variable UNREAL_ROOT to its engine directory.

Dispatch “Unreal engine verification” against the branch being reviewed. It compiles TheUnmadeGameEditor (including UHT) and executes the Unmade automation group, rejecting missing reports, zero passing tests, failures and skipped tests. Logs/reports are uploaded even on failure. A headless automation pass does not replace manual campaign, visuals, collision, UI and performance acceptance.

After a successful baseline, set UNREAL_CI_ENABLED to `true` to enable trusted push runs. Manually dispatch PR branches only after review: do not execute untrusted fork code on the persistent host. Configure the stable `editor-build-and-tests` job as a required check once runner availability is established. Missing runner execution is unverified, never a pass; do not enable a required check before provisioning its host.
