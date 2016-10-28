# POSIX `mkfifo(3)` for NodeJS

## Installation
```
npm install mkfifo
```

## Example

### Sync

```javascript
var mkfifoSync = require('mkfifo').mkfifoSync;

mkfifoSync('/path/to/fifo', 0755);
```

### Async

```javascript
var mkfifo = require('mkfifo').mkfifo;

mkfifo('/path/to/fifo', 0755, function(err) {
	/* ... */
});
```
