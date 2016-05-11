var fs = require('fs');


fs.readFile('index.html', 'utf8', function (err, data) {
    if (err) {
        return console.log(err);
    }
    var html = "'" +
        data.replace(/(\r\n|\n|\r|\t)/gm, "")
        .replace(/\'/gm, "\\'") + "'";

    fs.writeFile("html.min.cpp", html, function (err) {
        if (err) {
            return console.log(err);
        }

        console.log("The file was saved!");
    });
});