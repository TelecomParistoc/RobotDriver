
function ChartView(container) {
    var options = {
        axisX: {
            // The offset of the labels to the chart area
            offset: 40,
            position: 'end',
            lineSmooth: false,
            showPoint: false,
            // If labels should be shown or not
            showLabel: true,
            // If the axis grid should be drawn or not
            showGrid: true,
            // Interpolation function that allows you to intercept the value from the axis label
            labelInterpolationFnc: function(value) {
                if(Math.round(value*1000)%100 === 0)
                    return Math.round(value*100)/100;
                else
                    return '';
            }
        },
        showPoint: false,
        lineSmooth: false,
        fullWidth: true,
        chartPadding: {
            right: 40
        }
    };

    var data = [[0]];
    var chart = new Chartist.Line(container, {labels: [], series: data}, options);

    function render(labels, data) {
        chart.update({labels: labels, series: data});
    }

    return {
        render: render,
    };
}
