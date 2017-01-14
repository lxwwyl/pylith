#!/usr/bin/env python
#
# ----------------------------------------------------------------------
#
# Brad T. Aagaard, U.S. Geological Survey
# Charles A. Williams, GNS Science
# Matthew G. Knepley, University of Chicago
#
# This code was developed as part of the Computational Infrastructure
# for Geodynamics (http://geodynamics.org).
#
# Copyright (c) 2010-2016 University of California, Davis
#
# See COPYING for license information.
#
# ----------------------------------------------------------------------
#

# @file pylith/utils/DumpParameters.py
#
# @brief Python DumpParameters object for dumping PyLith parameter information to a file.

from pyre.components.Component import Component

# DumpParameters class


class DumpParameters(Component):
    """
    Python DumpParameters object for dumping PyLith parameter information to a file.
    """

    # PUBLIC METHODS /////////////////////////////////////////////////////

    def __init__(self, name):
        """
        Constructor.
        """
        Component.__init__(self, name="dumpparamters", facility="dumpparameters")
        self.info = None
        return

    def write(self, app):
        """
        Write parameters to ASCII file.
        """
        # Do nothing
        return

    def collect(self, app):
        """
        Collect version information and parameters.
        """
        from CollectVersionInfo import CollectVersionInfo
        import datetime
        self.info = CollectVersionInfo.asDict()
        self.info["timestamp"] = datetime.datetime.now().isoformat()
        (properties, components) = self._getPropertiesComponents(app)
        self.info["application"] = {
            "name": app.name,
            "class": str(app),
            "properties": properties,
            "components": components,
        }
        return

    # PRIVATE METHODS ////////////////////////////////////////////////////

    def _getPropertiesComponents(self, obj):
        """
        Get objects properties and components.
        """
        propertyNames = obj.inventory.propertyNames()
        propertyNames.sort()

        facilityNames = obj.inventory.facilityNames()
        facilityNames.sort()

        propertiesOmit = [
            "help",
            "help-components",
            "help-persistence",
            "help-properties",
            "typos",
        ]
        properties = {}
        for name in propertyNames:
            if name in facilityNames or name in propertiesOmit:
                continue
            trait = obj.inventory.getTrait(name)
            descriptor = obj.inventory.getTraitDescriptor(name)
            try:
                description = trait.meta['tip']
            except KeyError:
                description = "No description available."
            properties[name] = {
                "value": str(descriptor.value),
                "type": trait.type,
                "description": description,
                "setFrom": str(descriptor.locator),
            }

        facilitiesOmit = [
            "weaver",
        ]
        facilities = {}
        for name in facilityNames:
            if name in facilitiesOmit:
                continue
            trait = obj.inventory.getTrait(name)
            descriptor = obj.inventory.getTraitDescriptor(name)
            try:
                description = trait.meta['tip']
            except KeyError:
                description = "No description available."

            facilityProperties, facilityComponents = self._getPropertiesComponents(descriptor.value)
            facilities[name] = {
                "name": descriptor.value.name,
                "class": str(descriptor.value),
                "description": description,
                "setFrom": str(descriptor.locator),
                "aliases": descriptor.value.aliases,
                "properties": facilityProperties,
                "components": facilityComponents,
            }
        return (properties, facilities)


# FACTORIES ////////////////////////////////////////////////////////////

def dump_parameters():
    """
    Factory associated with DumpParameters.
    """
    return DumpParameters()

# End of file