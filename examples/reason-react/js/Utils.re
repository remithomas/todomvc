let uuid = () => {
	let uuid = ref("");

	let xStart = 0;
	let xEnd = 32;
	for (i in xStart to xEnd) {
		let random = Js.Math.random() *. 16.;

		if (i == 8 || i == 12 || i == 16 || i == 20) {
			uuid := uuid^ ++ "-";
		}

		uuid := uuid^ ++ Js.Float.toString(i == 12
			? 4.
			: (i == 16
				? float_of_int((lor)((land)(int_of_float(random), 3), 8))
				: random));
	}

	uuid^;
}

let sanatizeTodo = (text) => {
	Js.String.trim(text);
};

let filterTodos = (todos: array(Model.todo), filter: Model.filter) => {
	Belt.Array.keep(todos, (todo) => {
		switch(filter) {
			| All => true
			| Active => !todo.completed
			| Completed => todo.completed
		};
	});
};

let valueFromEvent = (event): string => event->ReactEvent.Form.target##value;
let booleanValueFromEvent = (event): bool => event->ReactEvent.Form.target##checked;

let pluralize = (count: int, word: string)  => {
	count == 1
		? word
		: word ++ "s";
}

module Decoder {
	let decodeTodo = json => {
		let todo: Model.todo = Json.Decode.{
			id: json |> field("id", string),
			title: json |> field("title", string),
			completed: json |> field("completed", bool)
		};
		todo;
	}

	let decodeTodos = Json.Decode.array(decodeTodo);
}

module Encoder {
	let encodeTodo = (todo: Model.todo) =>
		Json.Encode.object_([
			("id", Json.Encode.string(todo.id)),
			("title", Json.Encode.string(todo.title)),
			("completed", Json.Encode.bool(todo.completed)),
		]);

	let encodeTodos = todos =>
		todos
		|> Js.Array.map(encodeTodo)
		|> Json.Encode.jsonArray
		|> Json.stringify;
}

module Store {
	let getTodos = (): array(Model.todo) => {
		let strTodos = Dom.Storage.(localStorage |> getItem(Constant.storageKey));
		let jsonTodos = Belt.Option.getWithDefault(strTodos, "[]");

		Json.parseOrRaise(jsonTodos) |> Decoder.decodeTodos;
	}

	let saveTodos = (todos: array(Model.todo)) => {
		open Dom.Storage;
  		localStorage |> setItem(Constant.storageKey, Encoder.encodeTodos(todos));
	}
}
