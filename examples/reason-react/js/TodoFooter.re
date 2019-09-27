[@react.component]
let make = (
	~count,
	~completedCount,
	~nowShowing: Model.filter,
	~onClearCompleted
) => {
	let clearButton = completedCount == 0
		? ReasonReact.null
		: {
			<button
          		className="clear-completed"
				onClick={(_event) => onClearCompleted()}>
				{ReasonReact.string("Clear completed")}
        	</button>
		};

	let activeTodoSize = count - completedCount;

	<footer className="footer">
		<span className="todo-count">
			<strong>{React.string(string_of_int(activeTodoSize))}</strong> {ReasonReact.string(Utils.pluralize(activeTodoSize, " todo") ++ " left")}
        </span>
        <ul className="filters">
          <li>
            <a
              href="#/"
			  className={nowShowing === Model.All ? "selected" : ""}
			>
                {ReasonReact.string("All")}
            </a>
          </li>
          <li>
            <a
              href="#/active"
			  className={nowShowing === Model.Active ? "selected" : ""}
			>
                {ReasonReact.string("Active")}
            </a>
          </li>
          <li>
            <a
              href="#/completed"
			  className={nowShowing === Model.Completed ? "selected" : ""}
			>
                {ReasonReact.string("Completed")}
            </a>
          </li>
        </ul>
        {clearButton}
      </footer>
}
