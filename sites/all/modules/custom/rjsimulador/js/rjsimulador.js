(function ($) {
    Drupal.behaviors.rjSimBehaviour = {
        attach: function (context, settings) {
            generarEnlacesTabla("fila-datos", "dato", "#5CB8E6", "blue");
            generarEnlacesTabla("fila-infraccion", "infraccion", "#CD5252", "red");
            comprobarBoton();
        }
    };

    function comprobarBoton() {
        var listaDatos = $(".div-imagen").children();

        listaDatos.on('click',
            function() {
                var divHijo = $(this).children('.completo');
                if (divHijo.hasClass('oculto')) {
                    divHijo.parent().addClass('iluminado');
                    divHijo.removeClass('oculto');
                } else {
                    divHijo.parent().removeClass('iluminado');
                    divHijo.addClass('oculto');
                }
            }
        );
    }

    function generarEnlacesTabla(clase, claseImagen, colorHover, colorClick) {
        var filas = $("tr." + clase);
        filas.hover(
            function () {
                $(this).children("td").css('background-color', colorHover);
            },
            function () {
                $(this).children("td").css('background-color', 'white');
            }
        );

        filas.on("click", function () {
            var texto = $(this).children("td." + claseImagen + "-value").text();
            var textoReplace = texto.replace('.', '-');

            var elementoDatos = $("#" + claseImagen + "-" + textoReplace);

            $('html, body').animate({
                scrollTop: elementoDatos.offset().top - 20
            }, 100);

            $(".div-imagen").children().each(function() {
               if ($(this).hasClass('iluminado')) {
                   $(this).removeClass('iluminado');
               }
            });

            elementoDatos.addClass('iluminado');
        });

        filas.on("mousedown", function () {
            $(this).children("td").css('background-color', colorClick);
        });

        filas.on("mouseup", function () {
            $(this).children("td").css('background-color', colorHover);
        });
    }
})(jQuery);