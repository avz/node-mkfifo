# POSIX `mkfifo(3)` for NodeJS

## Installation
```
npm install mkfifo
```

## Example

```javascript
var mkfifoSync = require('mkfifo').mkfifoSync;

mkfifoSync('/path/to/fifo', 0755);
```
