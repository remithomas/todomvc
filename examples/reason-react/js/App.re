type state = {
	todos: array(Model.todo),
	editing: option(Model.todoId),
};

type actions =
	| AddTodo(string)
	| ToggleTodo(Model.todoId)
	| EditTodo(Model.todoId)
	| CancelEditTodo
	| SaveTodo(Model.todoId, string)
	| DeleteTodo(Model.todoId)
	| ToggleAll(bool)
	| ClearCompleted;

let initialState = {
	todos: Utils.Store.getTodos(),
	editing: None,
}

let reducer = (state, action) => {
	switch (action) {
		| AddTodo(title) => {
			let newTodo: Model.todo = {id: Utils.uuid(), title: Utils.sanatizeTodo(title), completed: false};
			let newTodos = Array.append(state.todos, [|newTodo|]);
			Utils.Store.saveTodos(newTodos);
			{...state, todos: newTodos};
		}
		| EditTodo(todoId) => {...state, editing: Some(todoId)}
		| SaveTodo(todoId, title) => {
			let newTodos = Array.map((todo: Model.todo) => {
				todo.id === todoId
					? {...todo, title}
					: todo
			}, state.todos);
			Utils.Store.saveTodos(newTodos);
			{ todos: newTodos, editing: None };
		}
		| CancelEditTodo => {...state, editing: None}
		| ToggleTodo(todoId) => {
			let newTodos = Array.map((todo: Model.todo) => {
				todo.id === todoId
					? {...todo, completed: !todo.completed}
					: todo
			}, state.todos);
			Utils.Store.saveTodos(newTodos);
			{...state, todos: newTodos};
		}
		| DeleteTodo(todoId) => {
			let newTodos = Belt.Array.keep(state.todos, todo => todo.id != todoId);
			Utils.Store.saveTodos(newTodos);
			{...state, todos: newTodos};
		}
		| ToggleAll(toggle) => {
			let newTodos = Array.map((todo: Model.todo) => {...todo, completed: toggle}, state.todos);
			Utils.Store.saveTodos(newTodos);
			{...state, todos: newTodos};
		}
		| ClearCompleted => {
			let newTodos = Belt.Array.keep(state.todos, todo => !todo.completed);
			Utils.Store.saveTodos(newTodos);
			{...state, todos: newTodos};
		}
	};
};

[@react.component]
let make = () => {
	let url = ReasonReactRouter.useUrl();

	let (state, dispatch) = React.useReducer(reducer, initialState);
	let (text, setText) = React.useReducer((_oldText, newText) => newText, "");

	let nowShowing = switch(url.hash) {
		| "/active" => Model.Active
		| "/completed" => Model.Completed
		| _ => Model.All
	};

	let todos = Utils.filterTodos(state.todos, nowShowing);
	let activeTodos = Utils.filterTodos(state.todos, Model.Active);
	let hasTodos = Array.length(state.todos) > 0;

	<div>
		<header className="header">
			<h1>{ReasonReact.string("todos")}</h1>

			<input
		       value=(text)
		       type_="text"
			   className="new-todo"
		       placeholder="What needs to be done?"
		       onChange=((event) => setText(Utils.valueFromEvent(event)))
		       onKeyDown=((event) =>
				if (ReactEvent.Keyboard.keyCode(event) == Constant.enter_key) {
					dispatch(AddTodo(text));
					setText("");
				}
		       )
			   autoFocus={true}
		     />
		</header>

		{
			if (hasTodos) {
				<>
					<section className="main">
						<input
							id="toggle-all"
							className="toggle-all"
							type_="checkbox"
							onChange={(event) => dispatch(ToggleAll(Utils.booleanValueFromEvent(event)))}
							checked={Array.length(activeTodos) === 0}
						/>
						<label htmlFor="toggle-all">
							{React.string("Mark all as complete")}
						</label>

						<ul className="todo-list">
							{Array.map((todo) => {
								<TodoItem
									key={todo.id}
									todo
									editing={state.editing == Some(todo.id)}
									onCancel={() => dispatch(CancelEditTodo)}
									onDestroy={() => dispatch(DeleteTodo(todo.id))}
									onToggle={() => dispatch(ToggleTodo(todo.id))}
									onEdit={() => dispatch(EditTodo(todo.id))}
									onSave={(title) => dispatch(SaveTodo(todo.id, title))}
								/>
							}, todos) |> React.array}
						</ul>
					</section>

					<TodoFooter
						count={Array.length(state.todos)}
						completedCount={Belt.Array.keep(state.todos, todo => todo.completed) |> Array.length}
						nowShowing={nowShowing}
						onClearCompleted={() => dispatch(ClearCompleted)}
					/>
				</>
			} else {
				ReasonReact.null;
			}
		}
	</div>
}
