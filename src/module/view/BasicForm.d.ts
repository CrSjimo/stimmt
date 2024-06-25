declare module 'stimmt:view' {

    export interface BasicFormWidgets {
        TextBox: {
            placeholder: string;
            value: string;
        };
        TextArea: {
            placeholder: string;
            value: string;
        };
        ComboBox: {
            items: string[];
            value: number;
        };
        CheckBox: {
            value: boolean;
        };
        Slider: {
            minimum: number;
            maximum: number;
            value: number;
        };
        SpinBox: {
            minimum: number;
            maximum: number;
            value: number;
        };
        DoubleSpinBox: {
            minimum: number;
            maximum: number;
            decimals: number;
            value: number;
        };
    }

    export class BasicForm {
        constructor();

        addRow<T extends keyof BasicFormWidgets>(id: string, label: string, type: T, widget: BasicFormWidgets[T]): void;
        addRow<T extends keyof BasicFormWidgets>(id: string, label: string, type: string, widget: any): void;

        rowValue(id: string): any;
        setRowValue(id: string, value: any): void;

        readonly rowValueChanged: Qt.Signal<[string, any]>;
    }
}