/*jslint browser: true*/
/*global jQuery, Drupal*/
/**
 * @file
 * Defines the manuscript-viewer widget.
 */
(function ($) {
  'use strict';

  /**
   * Cause all page redirects to occur in the parent of the iframe.
   */
  $(document).on('click', 'a', function(event) {
    var location = $(this).attr('href');
    var target = $(this).attr('target');
    if (typeof(location) != "undefined" &&
        location !== '#' &&
        target != "_blank") {
      event.preventDefault();
      top.location.replace(location);
    }
  });

  /**
   * The DOM element that represents the Singleton Instance of this class.
   * @type {string}
   */
  var base = '#livingstone-manuscript-viewer';

  /**
   * Initialize the Livingstone Manuscript Viewer.
   */
  Drupal.behaviors.livingstoneManuscriptViewer = {
    attach: function (context, settings) {
      if (Drupal.LivingstoneManuscriptViewer[base] === undefined) {
        $(base, document).once('livingstoneManuscriptViewer', function () {
          Drupal.LivingstoneManuscriptViewer[base] = new Drupal.LivingstoneManuscriptViewer(base, settings.livingstoneManuscriptViewer);
        });
      }
    },
    detach: function () {
      $(base).removeClass('livingstoneManuscriptViewer-processed');
      $(base).removeData();
      $(base).off();
      delete Drupal.LivingstoneManuscriptViewer[base];
    }
  };

  /**
   * Resize the viewer to fit the window.
   */
  function resize() {
    var height = window.innerHeight - $('#toolbar').outerHeight();
    $('#openseadragon, #item-details, #transcription').height(height);
    var width = window.innerWidth > 568 ? window.innerWidth / 2 : window.innerWidth;
    $('#item-details, #transcription').width(width);
  }

  /**
   * Scroll to the given element in the transcription pane.
   *
   * @param {int} page
   */
  function transcriptionScrollTo(page) {
    var element = $("span.pb-title:eq(" + page + ")");
    if (element.length != 0) {
      $('#transcription').animate({
        scrollTop: element[0].offsetTop + 'px'
      }, 1000);
    }
  }

  /**
   * Wrapper around OpenSeadragon.
   * @constructor
   */
  Drupal.LivingstoneManuscriptImageViewer = function (pid, initialPage, pages, options) {
    var that = this,
        openseadragon = new OpenSeadragon($.extend({
          element: $('#openseadragon', base).get(0),
          tileSources: $.map(pages, function (page) {
            return {
              pid: page.pid,
              token: page.token,
              width: page.width,
              height: page.height,
              maxLevel: page.levels
            };
          }),
          initialPage: initialPage,
          sequenceMode: true,
          showReferenceStrip: false,
          referenceStripPosition: 'BOTTOM',
          referenceStripSizeRatio: 0.1,
          showZoomControl: false,
          showHomeControl: false,
          showRotationControl: false,
          showFullPageControl: false,
          showSequenceControl: false,
          imageLoaderLimit: 5,
          zoomPerClick: 1.2,
          animationTime: 0,
          zoomPerScroll: 1.2,
          zoomPerSecond: 1.0,
          springStiffness: 1.0,
          viewportMargins: {
            left: 10,
            right: 10,
            top: 10,
            bottom: 30
          }
        }, options));

    /**
     * Notifies the parent frame that we have changed pages.
     *
     * @param {number} page
     *   The new page number.
     */
    function sendSetPageMessage(page) {
      parent.window.postMessage({
        event: 'page',
        pid: pid,
        page: page
      }, "*");
    }

    /**
     * Listen for page events.
     */
    function receiveMessage(event) {
      if (typeof event.data.event == "undefined") {
        return;
      }
      switch (event.data.event) {
        case 'page':
          if (typeof event.data.page != "undefined") {
            that.setPage(event.data.page);
          }
          else {
            that.setPage(initialPage);
          }
          break;
      }
    }

    /**
     * Checks if the given number is a valid page number.
     *
     * @param num {number}
     *   The page number to check.
     *
     * @return {bool}
     */
    function validPageNumber(num) {
      return $.isNumeric(num) && num >= 0 && num < pages.length;
    }

    /**
     * Zoom in a single unit.
     */
    function doSingleZoomIn() {
      var viewport = openseadragon.viewport;
      if ( viewport ) {
        viewport.zoomBy(
            openseadragon.zoomPerClick / 1.0
        );
        viewport.applyConstraints();
      }
    }

    /**
     * Zoom out a single unit.
     */
    function doSingleZoomOut() {
      var viewport = openseadragon.viewport;
      if ( viewport ) {
        viewport.zoomBy(
            1.0 / openseadragon.zoomPerClick
        );
        viewport.applyConstraints();
      }
    }

    /**
     * Get the current percentage of the zoom.
     *
     * @return {number}
     */
    function getZoomPercentage() {
      var viewport = openseadragon.viewport,
          min = viewport.getMinZoom(),
          max = viewport.getMaxZoom(),
          range = max - min,
          current = viewport.getZoom(true);
      return (current - min) / range;
    }

    /**
     * Sets the given page without firing any events or setting the state.
     */
    function setPage(page) {
      if (validPageNumber(page)) {
        $('select.page-select').val(page);
        openseadragon._sequenceIndex = page;
        openseadragon._updateSequenceButtons( page );
        openseadragon.open( openseadragon.tileSources[ page ] );
        if( openseadragon.referenceStrip ){
          openseadragon.referenceStrip.setFocus( page );
        }
      }
    }

    /**
     * Sets the value of the pager in the toolbar.
     */
    function setToolbarPage(page) {
      $('select.page-select').val(page);
    }

    /**
     * Setup the toolbar and bound actions to it.
     */
    function initializeToolbar() {
      $('#zoom-slider').slider({
        min: 1,
        max: 100,
        slide: function( event, ui ) {
          var viewport = openseadragon.viewport,
              min = viewport.getMinZoom(),
              max = viewport.getMaxZoom(),
              range = max - min,
              zoom = range * ( ui.value / 100);
          viewport.zoomTo(min + zoom);
        }
      });

      openseadragon.addHandler("animation", function () {
        var value = getZoomPercentage();
        $('#zoom-slider').slider('value', value * 100);
      });

      // Zoom Out.
      $('a.zoom-out.icon').click(function () {
        doSingleZoomOut();
      });

      // Zoom In.
      $('a.zoom-in.icon').click(function () {
        doSingleZoomIn();
      });

      // Rotate.
      $('a.rotate.icon').click(function () {
        var viewport = openseadragon.viewport,
            current = viewport.getRotation();
        viewport.setRotation((current + 90) % 360);
      });

      // Page.
      $('select.page-select').change(function () {
        openseadragon.goToPage(parseInt($(this).val()));
      });

      // Navigation prev.
      $('#openseadragon .prev-icon').click(function () {
        var page = openseadragon.currentPage() - 1;
        if (validPageNumber(page)) {
          openseadragon.goToPage(page);
        }
      });

      // Navigation next.
      $('#openseadragon .next-icon').click(function () {
        var page = openseadragon.currentPage() + 1;
        if (validPageNumber(page)) {
          openseadragon.goToPage(page);
        }
      })
    }

    /**
     * Setup the reference strip and bound actions to it.
     */
    function initializeTranscription() {
      $('.TEI span.pb-title').click(function () {
        var page = parseInt($(this).text().replace(/[a-z-_.:]*/gi, '')) - 1;
        if (validPageNumber(page)) {
          openseadragon.goToPage(page);
        }
      });
    }

    /**
     * Public functions.
     */
    this.currentPage = function () {
      return openseadragon.currentPage();
    };

    // Start up.
    initializeToolbar();
    initializeTranscription();
    setToolbarPage(initialPage);

    // Zoom Out.
    openseadragon.viewport.zoomTo(openseadragon.viewport.getMinZoom());

    /**
     * Handle Events.
     */
    openseadragon.addHandler("page", function (data) {
      setToolbarPage(data.page);
      sendSetPageMessage(data.page);
      transcriptionScrollTo(data.page);
    });

    openseadragon.addHandler('open', function() {
      resize();
      openseadragon.viewport.zoomTo(openseadragon.viewport.getMinZoom(), null, true);
      openseadragon.viewport.applyConstraints();
    });

    window.addEventListener("message", receiveMessage, false);

  };

  /**
   * Creates an instance of the Livingstone Manuscript Viewer widget.
   *
   * @param {string} base
   *   The element ID that this class is bound to.
   * @param {object} settings
   *   Drupal.settings for this object widget.
   *
   * @constructor
   */
  Drupal.LivingstoneManuscriptViewer = function (base, settings) {

    var that = this,
        pid = settings.pid,
        pages = settings.pages,
        initialPage = settings.initialPage,
        hasTranscription = settings.hasTranscription,
        viewer = null;

    // Only open the viewer if there are pages.
    if (pages.length > 0) {
      $('#restricted-message-wrapper').remove();
      viewer = new Drupal.LivingstoneManuscriptImageViewer(
          pid,
          initialPage,
          pages,
          settings.openSeaDragon.options
      );
    }
    else {
      // Hide the controls used with the viewer.
      $('.zoom-out, .zoom-slider, .zoom-in, .rotate, .page-select, .prev-icon, .next-icon').hide();
    }

    function toggleItemDetails() {
      var open = $('#openseadragon').hasClass('item-details-open');
      if (open) {
        hideItemDetails();
      }
      else {
        showItemDetails();
        if (window.innerWidth <= 568) {
          hideTranscription();
        }
      }
    }

    function showItemDetails() {
      $('#item-details').show();
      $('#openseadragon').addClass('item-details-open');
      $('.item-details.icon').addClass('depressed');
      resize();
    }

    function hideItemDetails() {
      $('#item-details').hide();
      $('#openseadragon').removeClass('item-details-open');
      $('.item-details.icon').removeClass('depressed');
      resize();
    }

    function toggleTranscription() {
      var open = $('#openseadragon').hasClass('transcription-open');
      if (open) {
        hideTranscription();
      }
      else {
        showTranscription();
        if (window.innerWidth <= 568) {
          hideItemDetails();
        }
      }
    }

    function showTranscription() {
      $('#transcription').show();
      $('#openseadragon').addClass('transcription-open');
      $('.transcription.icon').addClass('depressed');
      setTimeout(function () {
        if (viewer) {
          transcriptionScrollTo(viewer.currentPage());
        }
      }, 1000);
      resize();
    }

    function hideTranscription() {
      $('#transcription').hide();
      $('#openseadragon').removeClass('transcription-open');
      $('.transcription.icon').removeClass('depressed');
      resize();
    }

    /**
     * Setup the toolbar and bound actions to it.
     */
    function initializeToolbar() {
      // Item Details.
      $('.item-details.icon').click(toggleItemDetails);

      // Transcription.
      if (hasTranscription) {
        $('.transcription.icon').click(toggleTranscription);
      } else {
        $('.transcription.icon').addClass('disabled');
      }

      // Close
      $('.close.icon').click(function () {
        parent.window.postMessage({ event: 'close'}, "*");
      });
    }

    // Start up.
    initializeToolbar();
    resize();

    /**
     * Handle Events.
     */
    $(window).resize(function () {
      resize();
    });

    $(window).on("orientationchange", function(event) {
      resize();
    });

    /**
     * Wait to display.
     */
    setTimeout(function () {
      $('body').removeClass('loading');
    }, 3000);
  }

}(jQuery));