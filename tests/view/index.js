const { BasicForm, Quick } = require("stimmt:view")

let form = new BasicForm();

form.addRow("a", "AAA", "TextBox", {
    "placeholder": "text a",
});

let quick = new Quick();
quick.load(":/index.qml");

quick.messageReceived.connect((name, value) => {
    console.log(name, value);
})

module.exports = {form, quick};