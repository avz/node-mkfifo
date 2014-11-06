#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#include <node.h>

using namespace v8;

Handle<Value> MkfifoSync(const Arguments& args) {
	HandleScope scope;

	if (args.Length() < 2) {
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return scope.Close(Undefined());
	}

	if (!args[0]->IsString() || !args[1]->IsNumber()) {
		ThrowException(Exception::TypeError(String::New("Wrong arguments")));
		return scope.Close(Undefined());
	}


	mode_t mode = (mode_t)args[1]->Uint32Value();
	String::AsciiValue pathAscii(args[0]);

	if(mkfifo(*pathAscii, mode) != 0) {
		ThrowException(node::ErrnoException(errno, "mkfifo", strerror(errno), *pathAscii));

		return scope.Close(Undefined());
	}

	return scope.Close(Undefined());
}

void Init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("mkfifoSync"),
      FunctionTemplate::New(MkfifoSync)->GetFunction());
}

NODE_MODULE(mkfifo, Init)
