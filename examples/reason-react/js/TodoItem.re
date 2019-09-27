[@react.component]
let make = (
	~todo: Model.todo,
	~editing: bool,
	~onCancel,
	~onToggle,
	~onDestroy,
	~onSave,
	~onEdit
) => {
	let (editText, setEditText) = React.useReducer((_oldEditText, newEditText) => newEditText, "");

	let handleSubmit = (_event) => {
		onSave(editText);
	}
	let handleEdit = (_event) => {
		onEdit();
		setEditText(todo.title);
	};
	let handleKeyDown = (event) => {
		switch (ReactEvent.Keyboard.keyCode(event)) {
			| 13 => handleSubmit(event)
			| 27 => onCancel()
			| _ => ()
		}
	};
	let handleChange = (event) => setEditText(Utils.valueFromEvent(event));

	<li className={Cn.make([
		"completed"->Cn.ifTrue(todo.completed),
		"editing"->Cn.ifTrue(editing),
	])}>
		<div className="view">
          <input
            className="toggle"
            type_="checkbox"
            checked={todo.completed}
            onChange={(_event) => onToggle()}
          />
          <label onDoubleClick={(event) => handleEdit(event)}>
            {todo.title |> ReasonReact.string}
          </label>
          <button className="destroy" onClick={(_event) => onDestroy()} />
        </div>
        <input
        //   ref="editField"
          className="edit"
          value={editText}
          onBlur={(event) => handleSubmit(event)}
          onChange={(event) => handleChange(event)}
          onKeyDown={(event) => handleKeyDown(event)}
        />
	</li>
}
