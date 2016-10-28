# POSIX `mkfifo(3)` for NodeJS [![Build Status](https://travis-ci.org/avz/node-mkfifo.svg?branch=master)](https://travis-ci.org/avz/node-mkfifo)


## Installation
```
npm install mkfifo
```

## Example

### Sync

```javascript
var mkfifoSync = require('mkfifo').mkfifoSync;

mkfifoSync('/path/to/fifo', 0600);
```

### Async

```javascript
var mkfifo = require('mkfifo').mkfifo;

mkfifo('/path/to/fifo', 0600, function(err) {
	/* ... */
});
```
