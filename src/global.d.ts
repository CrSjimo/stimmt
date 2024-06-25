declare namespace Qt {
    interface Signal<Args extends any[]> {
        connect(f: (...args: Args) => void): void;
        connect(object: any, f: (...args: Args) => void): void;
        disconnect(f: (...args: Args) => void): void;
        disconnect(object: any, f: (...args: Args) => void): void;
    }
}

declare namespace Stimmt {
    type DataLike = string | ArrayBufferLike | ArrayBufferView;
}