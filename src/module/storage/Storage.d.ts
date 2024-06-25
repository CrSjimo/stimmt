declare module 'stimmt:storage' {
    export interface Storage {
        getItem(keyName: string): any;
        setItem(keyName: string, keyValue: any): void;
        removeItem(keyName: string): void;

        clear(): void;
        key(index: number): string | undefined;

        readonly size: number;
    }

    export const localStorage: Storage;
    export const sessionStorage: Storage;
    export const projectStorage: Storage;
    export const scopedStorage: Storage;
}