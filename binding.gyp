{
	"targets": [
		{
			"target_name": "mkfifo",
			"sources": ["src/mkfifo.cpp"],
			"include_dirs" : [
				"<!(node -e \"require('nan')\")"
			]
		}
	]
}
