const { BasicForm } = require("stimmt:view")

let form = new BasicForm();

form.addRow("a", "AAA", "TextBox", {
    "placeholder": "text a",
});

module.exports = form;