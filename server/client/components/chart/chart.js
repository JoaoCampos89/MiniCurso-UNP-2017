Template.chartComponent.helpers({
  create: function() {

  },
  rendered: function() {

  },
  destroyed: function() {

  },
});

Template.chartComponent.events({
  "click #foo": function(event, template) {

  }
});
/*Template.chartComponent.onRendered(function() {
  var n = 40,
    random = d3.random.normal(0, .2),
    data = d3.range(n).map(random);
  var margin = {
      top: 20,
      right: 20,
      bottom: 20,
      left: 40
    },
    width = 960 - margin.left - margin.right,
    height = 500 - margin.top - margin.bottom;

  var x = d3.scale.linear()
    .domain([0, n - 1])
    .range([0, width]);

  var y = d3.scale.linear()
    .domain([-1, 1])
    .range([height, 0]);

  var line = d3.svg.line()
    .x(function(d, i) {
      return x(i);
    })
    .y(function(d, i) {
      return y(d);
    });


  var svg = d3.select("#chart").append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

  svg.append("defs").append("clipPath")
    .attr("id", "clip")
    .append("rect")
    .attr("width", width)
    .attr("height", height);

  svg.append("g")
    .attr("class", "x axis")
    .attr("transform", "translate(0," + y(0) + ")")
    .call(d3.svg.axis().scale(x).orient("bottom"));

  svg.append("g")
    .attr("class", "y axis")
    .call(d3.svg.axis().scale(y).orient("left"));

  var path = svg.append("g")
    .attr("clip-path", "url(#clip)")
    .append("path")
    .datum(data)
    .attr("class", "line")
    .attr("d", line);

  tick();

  function tick() {
    // push a new data point onto the back
    data.push(random());
    // redraw the line, and slide it to the left
    path
      .attr("d", line)
      .attr("transform", null)
      .transition()
      .duration(200)
      .ease("linear")
      .attr("transform", "translate(" + x(-1) + ",0)")
      .each("end", tick);
    // pop the old data point off the front
    data.shift();
  }
});
*/

Template.chartComponent.onRendered(function() {
  var template = this;

  var data = Template.currentData().chartData;

  var margin = {
      top: 20,
      right: 20,
      bottom: 30,
      left: 50
    },
    width = 960 - margin.left - margin.right,
    height = 500 - margin.top - margin.bottom;

  //var formatDate = d3.time.format("%d-%b-%y");

  var x = d3.time.scale()
    .range([0, width]);

  var y = d3.scale.linear()
    .range([height, 0]);

  var xAxis = d3.svg.axis()
    .scale(x)
    .orient("bottom");

  var yAxis = d3.svg.axis()
    .scale(y)
    .orient("left");

  var line = d3.svg.line()
    .x(function(d) {
      return x(d.ts);
    })
    .y(function(d) {
      return y(d.measurament);
    });

  var svg = d3.select("#chart").append("svg")
    .attr("width", width + margin.left + margin.right)
    .attr("height", height + margin.top + margin.bottom)
    .append("g")
    .attr("transform", "translate(" + margin.left + "," + margin.top +
      ")");

  x.domain(d3.extent(data, function(d) {
    return d.ts;
  }));
  y.domain(d3.extent(data, function(d) {
    return d.measurament;
  }));

  svg.append("g")
    .attr("class", "x axis")
    .attr("transform", "translate(0," + height + ")")
    .call(xAxis);

  svg.append("g")
    .attr("class", "y axis")
    .call(yAxis)
    .append("text")
    .attr("transform", "rotate(-90)")
    .attr("y", 6)
    .attr("dy", ".71em")
    .style("text-anchor", "end")
    .text("Temperature");

  var path = svg.append("path")
    .data([data])
    .attr("class", "line")
    .attr("d", line);



  template.autorun(function() {
    data = Template.currentData().chartData;
    // Scale the range of the data again
    x.domain(d3.extent(data, function(d) {
      return d.ts;
    }));
    y.domain([0, d3.max(data, function(d) {
      return d.measurament;
    })]);

    // Select the section we want to apply our changes to

    svg.select(".x.axis") // change the x axis
      .call(xAxis);
    svg.select(".y.axis") // change the y axis
      .call(yAxis);
    svg.select("path.line").remove();
    svg.append("path")
      .data([data])
      .attr("class", "line")
      .attr("d", line);

  });


});
