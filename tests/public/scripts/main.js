Object.extend();
hljs.initHighlightingOnLoad();

var testCount  = 0;
function displayTest(test) {
    ChartView("#chart"+testCount).render(test.labels, test.data);
    ChartView("#Dchart"+testCount).render(test.labels, test.distances);
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
