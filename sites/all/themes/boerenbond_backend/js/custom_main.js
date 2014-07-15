;(function($) {
	// color values
	var green = "#aaffaa";
	var blue = "#2fa5e8";
	var red = "#faafbe";
	var yellow = "#edee78";
	var purple = "#c678ee";
	// on page load
	$(document).ready(function() {
		setColors();
	});
	// on ajax call
	$(document).ajaxComplete(function(event, xhr, settings) {
		setColors();
	});
	// function that gets the parent of class (table row) and sets style
	function setColors() {
		$(".Gepubliceerd").parents('tr').css('background', green);
		$(".redactieverantwoordelijke").parents('tr').css('background', blue);
		$(".Voorstel").parents('tr').css('background', red);
		$(".dossierbeheerder").parents('tr').css('background', purple);
		$(".hoofdredacteur").parents('tr').css('background', yellow);
	}
})(jQuery);
