id?String???Id of the element to append the viewer's container element to. If not provided, the 'element' property must be provided. If both the element and id properties are specified, the viewer is appended to the element provided in the element property.
element?Element???The element to append the viewer's container element to. If not provided, the 'id' property must be provided. If both the element and id properties are specified, the viewer is appended to the element provided in the element property.
tileSources?Array | String | function | Object?<optional>?null?Tile source(s) to open initially. This is a complex parameter; see OpenSeadragon.Viewer#open for details.
tabIndex?Number?<optional>?0?Tabbing order index to assign to the viewer element. Positive values are selected in increasing order. When tabIndex is 0 source order is used. A negative value omits the viewer from the tabbing order.
overlays?Array???Array of objects defining permanent overlays of the viewer. The overlays added via this option and later removed with OpenSeadragon.Viewer#removeOverlay will be added back when a new image is opened. To add overlays which can be definitively removed, one must use OpenSeadragon.Viewer#addOverlay If displaying a sequence of images, the overlays can be associated with a specific page by passing the overlays array to the page's tile source configuration. Expected properties: * x, y, (or px, py for pixel coordinates) to define the location. * width, height in point if using x,y or in pixels if using px,py. If width and height are specified, the overlay size is adjusted when zooming, otherwise the size stays the size of the content (or the size defined by CSS). * className to associate a class to the overlay * id to set the overlay element. If an element with this id already exists, it is reused, otherwise it is created. If not specified, a new element is created. * placement a string to define the relative position to the viewport. Only used if no width and height are specified. Default: 'TOP_LEFT'. See OpenSeadragon.Placement for possible values.
xmlPath?String?<optional>?null?DEPRECATED. A relative path to load a DZI file from the server. Prefer the newer Options.tileSources.
prefixUrl?String?<optional>?'/images/'?Prepends the prefixUrl to navImages paths, which is very useful since the default paths are rarely useful for production environments.
navImages?OpenSeadragon.NavImages?<optional>?An object with a property for each button or other built-in navigation control, eg the current 'zoomIn', 'zoomOut', 'home', and 'fullpage'. Each of those in turn provides an image path for each state of the button or navigation control, eg 'REST', 'GROUP', 'HOVER', 'PRESS'. Finally the image paths, by default assume there is a folder on the servers root path called '/images', eg '/images/zoomin_rest.png'. If you need to adjust these paths, prefer setting the option.prefixUrl rather than overriding every image path directly through this setting.
debugMode?Boolean?<optional>?false?TODO: provide an in-screen panel providing event detail feedback.
debugGridColor?String?<optional>?'#437AB2'?
blendTime?Number?<optional>?0?Specifies the duration of animation as higher or lower level tiles are replacing the existing tile.
alwaysBlend?Boolean?<optional>?false?Forces the tile to always blend. By default the tiles skip blending when the blendTime is surpassed and the current animation frame would not complete the blend.
autoHideControls?Boolean?<optional>?true?If the user stops interacting with the viewport, fade the navigation controls. Useful for presentation since the controls are by default floated on top of the image the user is viewing.
immediateRender?Boolean?<optional>?false?Render the best closest level first, ignoring the lowering levels which provide the effect of very blurry to sharp. It is recommended to change setting to true for mobile devices.
defaultZoomLevel?Number?<optional>?0?Zoom level to use when image is first opened or the home button is clicked. If 0, adjusts to fit viewer.
opacity?Number?<optional>?1?Default opacity of the tiled images (1=opaque, 0=transparent)
compositeOperation?String?<optional>?null?Valid values are 'source-over', 'source-atop', 'source-in', 'source-out', 'destination-over', 'destination-atop', 'destination-in', 'destination-out', 'lighter', 'copy' or 'xor'
placeholderFillStyle?String | CanvasGradient | CanvasPattern | function?<optional>?null?Draws a colored rectangle behind the tile if it is not loaded yet. You can pass a CSS color value like "#FF8800". When passing a function the tiledImage and canvas context are available as argument which is useful when you draw a gradient or pattern.
degrees?Number?<optional>?0?Initial rotation.
minZoomLevel?Number?<optional>?null?
maxZoomLevel?Number?<optional>?null?
homeFillsViewer?Boolean?<optional>?false?Make the 'home' button fill the viewer and clip the image, instead of fitting the image to the viewer and letterboxing.
panHorizontal?Boolean?<optional>?true?Allow horizontal pan.
panVertical?Boolean?<optional>?true?Allow vertical pan.
constrainDuringPan?Boolean?<optional>?false?
wrapHorizontal?Boolean?<optional>?false?Set to true to force the image to wrap horizontally within the viewport. Useful for maps or images representing the surface of a sphere or cylinder.
wrapVertical?Boolean?<optional>?false?Set to true to force the image to wrap vertically within the viewport. Useful for maps or images representing the surface of a sphere or cylinder.
minZoomImageRatio?Number?<optional>?0.9?The minimum percentage ( expressed as a number between 0 and 1 ) of the viewport height or width at which the zoom out will be constrained. Setting it to 0, for example will allow you to zoom out infinity.
maxZoomPixelRatio?Number?<optional>?1.1?The maximum ratio to allow a zoom-in to affect the highest level pixel ratio. This can be set to Infinity to allow 'infinite' zooming into the image though it is less effective visually if the HTML5 Canvas is not availble on the viewing device.
smoothTileEdgesMinZoom?Number?<optional>?1.1?A zoom percentage ( where 1 is 100% ) of the highest resolution level. When zoomed in beyond this value alternative compositing will be used to smooth out the edges between tiles. This will have a performance impact. Can be set to Infinity to turn it off. Note: This setting is ignored on iOS devices due to a known bug (See https://github.com/openseadragon/openseadragon/issues/952)
iOSDevice?Boolean?<optional>???True if running on an iOS device, false otherwise. Used to disable certain features that behave differently on iOS devices.
autoResize?Boolean?<optional>?true?Set to false to prevent polling for viewer size changes. Useful for providing custom resize behavior.
preserveImageSizeOnResize?Boolean?<optional>?false?Set to true to have the image size preserved when the viewer is resized. This requires autoResize=true (default).
minScrollDeltaTime?Number?<optional>?50?Number of milliseconds between canvas-scroll events. This value helps normalize the rate of canvas-scroll events between different devices, causing the faster devices to slow down enough to make the zoom control more manageable.
pixelsPerWheelLine?Number?<optional>?40?For pixel-resolution scrolling devices, the number of pixels equal to one scroll line.
visibilityRatio?Number?<optional>?0.5?The percentage ( as a number from 0 to 1 ) of the source image which must be kept within the viewport. If the image is dragged beyond that limit, it will 'bounce' back until the minimum visibility ratio is achieved. Setting this to 0 and wrapHorizontal ( or wrapVertical ) to true will provide the effect of an infinitely scrolling viewport.
viewportMargins?Object?<optional>?{}?Pushes the "home" region in from the sides by the specified amounts. Possible subproperties (Numbers, in screen coordinates): left, top, right, bottom.
imageLoaderLimit?Number?<optional>?0?The maximum number of image requests to make concurrently. By default it is set to 0 allowing the browser to make the maximum number of image requests in parallel as allowed by the browsers policy.
clickTimeThreshold?Number?<optional>?300?The number of milliseconds within which a pointer down-up event combination will be treated as a click gesture.
clickDistThreshold?Number?<optional>?5?The maximum distance allowed between a pointer down event and a pointer up event to be treated as a click gesture.
dblClickTimeThreshold?Number?<optional>?300?The number of milliseconds within which two pointer down-up event combinations will be treated as a double-click gesture.
dblClickDistThreshold?Number?<optional>?20?The maximum distance allowed between two pointer click events to be treated as a double-click gesture.
springStiffness?Number?<optional>?6.5?
animationTime?Number?<optional>?1.2?Specifies the animation duration per each OpenSeadragon.Spring which occur when the image is dragged or zoomed.
zoomPerClick?Number?<optional>?2.0?The "zoom distance" per mouse click or touch tap. Note: Setting this to 1.0 effectively disables the click-to-zoom feature (also see gestureSettings[Mouse|Touch|Pen].clickToZoom/dblClickToZoom).
zoomPerScroll?Number?<optional>?1.2?The "zoom distance" per mouse scroll or touch pinch. Note: Setting this to 1.0 effectively disables the mouse-wheel zoom feature (also see gestureSettings[Mouse|Touch|Pen].scrollToZoom}).
zoomPerSecond?Number?<optional>?1.0?The number of seconds to animate a single zoom event over.
showNavigator?Boolean?<optional>?false?Set to true to make the navigator minimap appear.
navigatorId?String?<optional>?navigator-GENERATED DATE?The ID of a div to hold the navigator minimap. If an ID is specified, the navigatorPosition, navigatorSizeRatio, navigatorMaintainSizeRatio, navigator[Top|Left|Height|Width] and navigatorAutoFade options will be ignored. If an ID is not specified, a div element will be generated and placed on top of the main image.
navigatorPosition?String?<optional>?'TOP_RIGHT'?Valid values are 'TOP_LEFT', 'TOP_RIGHT', 'BOTTOM_LEFT', 'BOTTOM_RIGHT', or 'ABSOLUTE'. If 'ABSOLUTE' is specified, then navigator[Top|Left|Height|Width] determines the size and position of the navigator minimap in the viewer, and navigatorSizeRatio and navigatorMaintainSizeRatio are ignored. For 'TOP_LEFT', 'TOP_RIGHT', 'BOTTOM_LEFT', and 'BOTTOM_RIGHT', the navigatorSizeRatio or navigator[Height|Width] values determine the size of the navigator minimap.
navigatorSizeRatio?Number?<optional>?0.2?Ratio of navigator size to viewer size. Ignored if navigator[Height|Width] are specified.
navigatorMaintainSizeRatio?Boolean?<optional>?false?If true, the navigator minimap is resized (using navigatorSizeRatio) when the viewer size changes.
navigatorTop?Number | String?<optional>?null?Specifies the location of the navigator minimap (see navigatorPosition).
navigatorLeft?Number | String?<optional>?null?Specifies the location of the navigator minimap (see navigatorPosition).
navigatorHeight?Number | String?<optional>?null?Specifies the size of the navigator minimap (see navigatorPosition). If specified, navigatorSizeRatio and navigatorMaintainSizeRatio are ignored.
navigatorWidth?Number | String?<optional>?null?Specifies the size of the navigator minimap (see navigatorPosition). If specified, navigatorSizeRatio and navigatorMaintainSizeRatio are ignored.
navigatorAutoResize?Boolean?<optional>?true?Set to false to prevent polling for navigator size changes. Useful for providing custom resize behavior. Setting to false can also improve performance when the navigator is configured to a fixed size.
navigatorAutoFade?Boolean?<optional>?true?If the user stops interacting with the viewport, fade the navigator minimap. Setting to false will make the navigator minimap always visible.
navigatorRotate?Boolean?<optional>?true?If true, the navigator will be rotated together with the viewer.
controlsFadeDelay?Number?<optional>?2000?The number of milliseconds to wait once the user has stopped interacting with the interface before begining to fade the controls. Assumes showNavigationControl and autoHideControls are both true.
controlsFadeLength?Number?<optional>?1500?The number of milliseconds to animate the controls fading out.
maxImageCacheCount?Number?<optional>?200?The max number of images we should keep in memory (per drawer).
timeout?Number?<optional>?30000?
useCanvas?Boolean?<optional>?true?Set to false to not use an HTML canvas element for image rendering even if canvas is supported.
minPixelRatio?Number?<optional>?0.5?The higher the minPixelRatio, the lower the quality of the image that is considered sufficient to stop rendering a given zoom level. For example, if you are targeting mobile devices with less bandwith you may try setting this to 1.5 or higher.
mouseNavEnabled?Boolean?<optional>?true?Is the user able to interact with the image via mouse or touch. Default interactions include draging the image in a plane, and zooming in toward and away from the image.
showNavigationControl?Boolean?<optional>?true?Set to false to prevent the appearance of the default navigation controls. Note that if set to false, the customs buttons set by the options zoomInButton, zoomOutButton etc, are rendered inactive.
navigationControlAnchor?OpenSeadragon.ControlAnchor?<optional>?TOP_LEFT?Placement of the default navigation controls. To set the placement of the sequence controls, see the sequenceControlAnchor option.
showZoomControl?Boolean?<optional>?true?If true then + and - buttons to zoom in and out are displayed. Note: OpenSeadragon.Options.showNavigationControl is overriding this setting when set to false.
showHomeControl?Boolean?<optional>?true?If true then the 'Go home' button is displayed to go back to the original zoom and pan. Note: OpenSeadragon.Options.showNavigationControl is overriding this setting when set to false.
showFullPageControl?Boolean?<optional>?true?If true then the 'Toggle full page' button is displayed to switch between full page and normal mode. Note: OpenSeadragon.Options.showNavigationControl is overriding this setting when set to false.
showRotationControl?Boolean?<optional>?false?If true then the rotate left/right controls will be displayed as part of the standard controls. This is also subject to the browser support for rotate (e.g. viewer.drawer.canRotate()). Note: OpenSeadragon.Options.showNavigationControl is overriding this setting when set to false.
showSequenceControl?Boolean?<optional>?true?If sequenceMode is true, then provide buttons for navigating forward and backward through the images.
sequenceControlAnchor?OpenSeadragon.ControlAnchor?<optional>?TOP_LEFT?Placement of the default sequence controls.
navPrevNextWrap?Boolean?<optional>?false?If true then the 'previous' button will wrap to the last image when viewing the first image and the 'next' button will wrap to the first image when viewing the last image.
zoomInButton?String???Set the id of the custom 'Zoom in' button to use. This is useful to have a custom button anywhere in the web page. To only change the button images, consider using OpenSeadragon.Options.navImages
zoomOutButton?String???Set the id of the custom 'Zoom out' button to use. This is useful to have a custom button anywhere in the web page. To only change the button images, consider using OpenSeadragon.Options.navImages
homeButton?String???Set the id of the custom 'Go home' button to use. This is useful to have a custom button anywhere in the web page. To only change the button images, consider using OpenSeadragon.Options.navImages
fullPageButton?String???Set the id of the custom 'Toggle full page' button to use. This is useful to have a custom button anywhere in the web page. To only change the button images, consider using OpenSeadragon.Options.navImages
rotateLeftButton?String???Set the id of the custom 'Rotate left' button to use. This is useful to have a custom button anywhere in the web page. To only change the button images, consider using OpenSeadragon.Options.navImages
rotateRightButton?String???Set the id of the custom 'Rotate right' button to use. This is useful to have a custom button anywhere in the web page. To only change the button images, consider using OpenSeadragon.Options.navImages
previousButton?String???Set the id of the custom 'Previous page' button to use. This is useful to have a custom button anywhere in the web page. To only change the button images, consider using OpenSeadragon.Options.navImages
nextButton?String???Set the id of the custom 'Next page' button to use. This is useful to have a custom button anywhere in the web page. To only change the button images, consider using OpenSeadragon.Options.navImages
sequenceMode?Boolean?<optional>?false?Set to true to have the viewer treat your tilesources as a sequence of images to be opened one at a time rather than all at once.
initialPage?Number?<optional>?0?If sequenceMode is true, display this page initially.
preserveViewport?Boolean?<optional>?false?If sequenceMode is true, then normally navigating through each image resets the viewport to 'home' position. If preserveViewport is set to true, then the viewport position is preserved when navigating between images in the sequence.
preserveOverlays?Boolean?<optional>?false?If sequenceMode is true, then normally navigating through each image resets the overlays. If preserveOverlays is set to true, then the overlays added with OpenSeadragon.Viewer#addOverlay are preserved when navigating between images in the sequence. Note: setting preserveOverlays overrides any overlays specified in the global "overlays" option for the Viewer. It's also not compatible with specifying per-tileSource overlays via the options, as those overlays will persist even after the tileSource is closed.
showReferenceStrip?Boolean?<optional>?false?If sequenceMode is true, then display a scrolling strip of image thumbnails for navigating through the images.
referenceStripScroll?String?<optional>?'horizontal'?
referenceStripElement?Element?<optional>?null?
referenceStripHeight?Number?<optional>?null?
referenceStripWidth?Number?<optional>?null?
referenceStripPosition?String?<optional>?'BOTTOM_LEFT'?
referenceStripSizeRatio?Number?<optional>?0.2?
collectionMode?Boolean?<optional>?false?Set to true to have the viewer arrange your TiledImages in a grid or line.
collectionRows?Number?<optional>?3?If collectionMode is true, specifies how many rows the grid should have. Use 1 to make a line. If collectionLayout is 'vertical', specifies how many columns instead.
collectionColumns?Number?<optional>?0?If collectionMode is true, specifies how many columns the grid should have. Use 1 to make a line. If collectionLayout is 'vertical', specifies how many rows instead. Ignored if collectionRows is not set to a falsy value.
collectionLayout?String?<optional>?'horizontal'?If collectionMode is true, specifies whether to arrange vertically or horizontally.
collectionTileSize?Number?<optional>?800?If collectionMode is true, specifies the size, in viewport coordinates, for each TiledImage to fit into. The TiledImage will be centered within a square of the specified size.
collectionTileMargin?Number?<optional>?80?If collectionMode is true, specifies the margin, in viewport coordinates, between each TiledImage.
crossOriginPolicy?String | Boolean?<optional>?false?Valid values are 'Anonymous', 'use-credentials', and false. If false, canvas requests will not use CORS, and the canvas will be tainted.
ajaxWithCredentials?Boolean?<optional>?false?Whether to set the withCredentials XHR flag for AJAX requests (when loading tile sources). Note that this can be overridden at the OpenSeadragon.TileSource level.
