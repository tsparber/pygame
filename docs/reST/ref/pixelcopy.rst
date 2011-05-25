.. include:: common.txt

:mod:`pygame.pixelcopy`
=======================

.. module:: pygame.pixelcopy
   :synopsis: pygame module for general pixel array copying

| :sl:`pygame module for general pixel array copying`

The :mod:`pygame.pixelcopy` module contains methods for copying between
surfaces and objects exporting an array structure interface.

New in pygame 1.9.2.

.. function:: surface_to_array

   | :sl:`copy surface pixels to an array object`
   | :sg:`surface_to_array(array, surface, kind='P', opaque=255, clear=0) -> None`

   Copy a surface to an array object.

   .. ## pygame.pixelcopy.surface_to_array ##

.. function:: array_to_surface

   | :sl:`copy an array object to a surface`
   | :sg:`array_to_surface(<surface>, <array>) -> None`

   Copy an array to a surface.

   .. ## pygame.pixelcopy.array_to_surface ##

.. function:: map_array

   | :sl:`copy an array to another array, using surface format`
   | :sg:`map_array(<array>, <array>, <surface>) -> None`

   Map an array of color element values - (w, h, ..., 3) - to an array of
   pixels - (w, h) according to the format of <<urface>.

   .. ## pygame.pixelcopy.map_array ##

.. function:: make_surface

   | :sl:`Copy an array to a new surface`
   | :sg:`pygame.surfarray.make_surface(array) -> Surface`

   Create a new Surface that best resembles the data and format on the array.
   The array can be 2D or 3D with any sized integer values.

   .. ## pygame.pixelcopy.make_surface ##

.. ## pygame.pixelcopy ##
