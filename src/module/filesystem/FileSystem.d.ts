declare module 'stimmt:filesystem' {
    export function readDir(dir: string): string[];
    export function exists(path: string): boolean;
    export function isDir(path: string): boolean;

    export function readFile(path: string): Uint8Array | undefined;
    export function writeFile(path: string, data: Stimmt.DataLike): void;
    export function appendFile(path: string, data: Stimmt.DataLike): void;

    export function remove(path: string): void;
    export function removeRecursively(dir: string): void;
    export function rename(oldPath: string, newPath: string): void;
    export function copyFile(src: string, dest: string): void;

    export function mkdir(dir: string): void;
    export function mkpath(dir: string): void;
    
    export function openFileOrUrl(path: string): void;
}