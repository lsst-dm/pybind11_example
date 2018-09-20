/*
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "ndarray/pybind11.h"

#include "lsst/utils/python.h"
#include "lsst/tmpl/ExampleOne.h"

namespace py = pybind11;
using namespace pybind11::literals;

namespace lsst { namespace tmpl {

void wrapExampleOne(utils::python::WrapperCollection & wrappers) {

    wrappers.addInheritanceDependency("lsst.pex.exceptions");

    wrappers.wrapException<ExampleError, pex::exceptions::RuntimeError>("ExampleError", "RuntimeError");

    auto clsExampleOne = wrappers.wrapType(
        py::class_<ExampleOne, std::shared_ptr<ExampleOne>>(wrappers.module, "ExampleOne"),
        [](auto & mod, auto & cls) {
            cls.def(py::init<>());
            cls.def(py::init<std::string const&, ExampleOne::State>(), "fileName"_a, "state"_a=ExampleOne::State::RED);
            cls.def(py::init<ExampleOne const&, bool>(), "other"_a, "deep"_a=true); // Copy constructor
            cls.def("getState", &ExampleOne::getState);
            cls.def("setState", &ExampleOne::setState);
            cls.def_property("state", &ExampleOne::getState, &ExampleOne::setState);
            cls.def("computeSomething", &ExampleOne::computeSomething);
            cls.def("computeSomethingElse",
                    py::overload_cast<int, double>(&ExampleOne::computeSomethingElse, py::const_),
                    "myFirstParam"_a, "mySecondParam"_a);
            cls.def("computeSomethingElse",
                    py::overload_cast<int, std::string>(&ExampleOne::computeSomethingElse, py::const_),
                    "myFirstParam"_a, "anotherParam"_a="foo");
            cls.def("computeSomeVector", &ExampleOne::computeSomeVector);
            cls.def("doSomethingWithArray", &ExampleOne::doSomethingWithArray);
            cls.def_static("initializeSomething", &ExampleOne::initializeSomething);
            cls.def("__eq__", &ExampleOne::operator==, py::is_operator());
            cls.def("__ne__", &ExampleOne::operator!=, py::is_operator());
            cls.def("__iadd__", &ExampleOne::operator+=);
            cls.def("__add__",
                    [](ExampleOne const & self, ExampleOne const & other) {
                        return self + other;
                    },
                    py::is_operator());
        }
    );

    wrappers.wrapType(
        py::enum_<ExampleOne::State>(clsExampleOne, "State"),
        [](auto & mod, auto & enm) {
            enm.value("RED", ExampleOne::State::RED);
            enm.value("ORANGE", ExampleOne::State::ORANGE);
            enm.value("GREEN", ExampleOne::State::GREEN);
            enm.export_values();
        }
    );

}

}}  // namespace lsst::tmpl
