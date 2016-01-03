Object.extend();
hljs.initHighlightingOnLoad();

var testCount  = 0;
function displayTest(test) {
    var chart = ChartView("#chart"+testCount);
    chart.render(test.labels, test.data);
    testCount++;
}
$(function () {
    $.get("/tests", function( data ) {
        var tests = JSON.parse(data);
        for(var i in tests) {
            displayTest(tests[i]);
        }
    });
});
