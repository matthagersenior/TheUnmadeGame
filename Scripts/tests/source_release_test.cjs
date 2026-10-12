const fs = require('node:fs');
const assert = require('node:assert/strict');
const workflow = fs.readFileSync('.github/workflows/source-release.yml', 'utf8');
const script = workflow.split('          script: |\n')[1].split('\n').map(line => line.replace(/^            /, '')).join('\n');
const execute = new (Object.getPrototypeOf(async function() {}).constructor)('context', 'github', 'core', 'process', script);
async function scenario({existing, published = false, green = true, manual = false} = {}) {
  const calls = [];
  const missing = () => { throw Object.assign(new Error('missing'), {status: 404}); };
  const github = {rest: {
    actions: {listWorkflowRuns: async () => ({data: {workflow_runs: green ? [{status: 'completed', conclusion: 'success'}] : []}})},
    repos: {
      getReleaseByTag: async () => published ? {data: {}} : missing(),
      createRelease: async args => { calls.push(['release', args]); }
    },
    git: {
      getRef: async () => existing ? {data: {object: {type: 'tag', sha: 'tag-object'}}} : missing(),
      getTag: async () => ({data: {object: {type: 'commit', sha: existing}}}),
      createTag: async args => { calls.push(['tag', args]); return {data: {sha: 'new-tag'}}; },
      createRef: async args => { calls.push(['ref', args]); }
    }
  }};
  await execute({ref: 'refs/heads/main', sha: 'verified', eventName: manual ? 'workflow_dispatch' : 'workflow_run', repo: {owner: 'owner', repo: 'repo'}, payload: {workflow_run: {head_sha: 'verified'}}}, github, {info() {}}, {env: {RELEASE_TAG: 'source-v18'}});
  return calls;
}
(async () => {
  assert.deepEqual((await scenario()).map(call => call[0]), ['tag', 'ref', 'release']);
  assert.deepEqual((await scenario({existing: 'verified'})).map(call => call[0]), ['release']);
  assert.deepEqual(await scenario({published: true}), []);
  await assert.rejects(scenario({existing: 'different'}), /different commit/);
  await assert.rejects(scenario({green: false}), /no successful source CI/);
  assert.equal((await scenario({manual: true, existing: 'verified'}))[0][1].tag_name, 'source-v18');
  console.log('PASS: six release publication and recovery scenarios');
})().catch(error => { console.error(error); process.exitCode = 1; });
