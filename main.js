var binding = require(__dirname + '/build/Release/mkfifo');

exports.mkfifoSync = binding.mkfifoSync;
exports.mkfifo = binding.mkfifo;
