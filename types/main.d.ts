export function mkfifoSync(path: string, mode: number): void;
export function mkfifo(path: string, mode: number, callback: (err: ({ errno: number, errstr: string } | undefined)) => void): void;
