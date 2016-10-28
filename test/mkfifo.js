var mkfifo = require('../');
var os = require('os');
var fs = require('fs');
var path = require('path');
var crypto = require('crypto');

function checkFIFOSync(path, mode, test) {
	var stat = fs.statSync(path);

	test.ok(stat.isFIFO());
	test.strictEqual(stat.mode & 0777, mode);
};

module.exports = {
	setUp: function(cb) {
		this.tmppath = path.join(
			os.tmpdir(),
			'.node-mkfifo.tmp.' + Date.now() + '.' + crypto.randomBytes(8).toString('hex')
		);

		cb();
	},
	tearDown: function(cb) {
		try {
			fs.unlinkSync(this.tmppath);
		} catch (e) {

		}

		cb();
	},
	mkfifoSync_mode1: function(test) {
		var mode = 0644;
		mkfifo.mkfifoSync(this.tmppath, mode);
		checkFIFOSync(this.tmppath, mode, test);

		test.done();
	},
	mkfifoSync_mode2: function(test) {
		var mode = 0600;

		mkfifo.mkfifoSync(this.tmppath, mode);
		checkFIFOSync(this.tmppath, mode, test);

		test.done();
	},
	mkfifoSync_error: function(test) {
		test.throws(
			function() {
				mkfifo.mkfifoSync(this.tmppath + '/nonexistent', 0644);
			},
			Error,
			'ENOENT'
		);

		test.done();
	},
	mkfifoAsync_mode1: function(test) {
		var self = this;
		var mode = 0644;

		mkfifo.mkfifo(this.tmppath, mode, function(error) {
			test.strictEqual(error, undefined);
			checkFIFOSync(self.tmppath, mode, test);

			test.done();
		});
	},
	mkfifoAsync_mode2: function(test) {
		var self = this;
		var mode = 0600;

		mkfifo.mkfifo(this.tmppath, mode, function(error) {
			test.strictEqual(error, undefined);
			checkFIFOSync(self.tmppath, mode, test);

			test.done();
		});
	},
	mkfifoAsync_error: function(test) {
		mkfifo.mkfifo(this.tmppath + '/nonexistent', 0644, function(error) {
			test.ok(error instanceof Error);
			test.strictEqual(error.code, 'ENOENT');
			test.done();
		});
	}
};
