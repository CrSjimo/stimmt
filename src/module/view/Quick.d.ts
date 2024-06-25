declare module 'stimmt:view' {
    export class Quick {
        constructor();

        load(filePath: string);
        sendMessage(name: string, message: any);

        readonly messageReceived: Qt.Signal<[string, any]>;
    }
}