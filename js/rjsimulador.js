(function ($) {
    Drupal.behaviors.rjSimBehaviour = {
        attach: function (context, settings) {
            generarEnlacesTabla("fila-datos", "#5CB8E6", "blue");
            generarEnlacesTabla("fila-infraccion", "#CD5252", "red");
        }
    };

    function generarEnlacesTabla(clase, colorHover, colorClick) {
        var filasInfraccion = $("tr." + clase);
        filasInfraccion.hover(
            function () {
                $(this).children("td").css('background-color', colorHover);
            },
            function () {
                $(this).children("td").css('background-color', 'white');
            }
        );

        filasInfraccion.on("click", function () {
        });

        filasInfraccion.on("mousedown", function () {
            $(this).children("td").css('background-color', colorClick);
        });

        filasInfraccion.on("mouseup", function () {
            $(this).children("td").css('background-color', colorHover);
        });
    }
})(jQuery);