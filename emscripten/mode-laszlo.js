define(function(require, exports, module) {
    var oop = require("ace/lib/oop");
    var TextMode = require("ace/mode/text").Mode;
    var TextHighlightRules = require("ace/mode/text_highlight_rules").TextHighlightRules;

    var MyLanguageHighlightRules = function() {
        this.$rules = {
            "start": [
                {
                    token: "keyword",
                    regex: "\\b(?:Array|Bool|Function|I32|Char|Range|String|and|assert|break|continue|else|false|for|function|if|in|let|mod|or|println|print|return|true|typeof|while|split|delete|new|struct|write|read)"
                },
                {
                    token : "string", // single line
                    regex : '["](?:(?:\\\\.)|(?:[^"\\\\]))*?["]'
                },
                {
                    token : "comment",
                    regex : "//.*$"
                }
            ]
        };
    };
    oop.inherits(MyLanguageHighlightRules, TextHighlightRules);

    var MyLanguageMode = function() {
        this.HighlightRules = MyLanguageHighlightRules;
    };
    oop.inherits(MyLanguageMode, TextMode);

    module.exports = MyLanguageMode;
});
