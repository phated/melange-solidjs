module Props = {
  [@deriving abstract]
  type domProps = {
    [@optional]
    onClick: Dom.mouseEvent => unit,
  };

  [@deriving abstract]
  type props;
};

module React = {
  type element;

  type componentLike('props, 'return) = 'props => 'return;

  type component('props) = componentLike('props, element);

  /* this function exists to prepare for making `component` abstract */
  external component: componentLike('props, element) => component('props) =
    "%identity";

  [@module]
  external createElement: (component('props), 'props) => element =
    "solid-js/h";
};

module ReactDOMRe = {
  include Props;

  [@module]
  external createElement:
    (string, ~props: Props.props=?, unit => React.element) => React.element =
    "solid-js/h";

  [@module]
  external createDOMElementVariadic:
    (string, ~props: Props.domProps=?, array(unit => React.element)) =>
    React.element =
    "solid-js/h";
};

module SolidJS = {
  [@module "solid-js/web"]
  external render: (React.element, Dom.element) => unit = "render";

  [@module "solid-js"]
  external createSignal: 'a => (unit => 'a, 'a => 'a) = "createSignal";

  external string: string => React.element = "%identity";
};

external querySelector: string => option(Dom.element) =
  "document.querySelector";

module App = {
  [@react.component]
  let make = () => {
    let (firstName, setFirstName) = SolidJS.createSignal("Blaine");

    <div>
      {() => SolidJS.string("Hello " ++ firstName())}
      {() =>
         <button onClick={evt => ignore(setFirstName("Unknown"))}>
           {() => SolidJS.string("Not " ++ firstName() ++ "?")}
         </button>}
    </div>;
  };
};

switch (querySelector("#root")) {
| Some(root) => SolidJS.render(<App />, root)
| None =>
  Js.Console.error("Failed to start Solid: couldn't find the #root element")
};
