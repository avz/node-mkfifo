import test from 'ava'
const mkfifo = require('../')
const fs = require('fs-extra')
const path = require('path')

var testTmpDir
var pipePath

test.beforeEach(t => {
  testTmpDir = fs.mkdtempSync(path.join(__dirname, 'tmp-'))
  pipePath = path.join(testTmpDir, 'test-pipe')
  fs.emptyDirSync(testTmpDir)
})

test.afterEach(t => {
  fs.removeSync(testTmpDir)
})

test('creates a pipe', t => {
  mkfifo.mkfifoSync(pipePath, 0o644)
  const stats = fs.statSync(pipePath)
  t.truthy(stats.isFIFO())
})
