#include <assert.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

size_t solve(const vector<size_t>& case_data) {
	const size_t n = case_data.size();

	/// widest_from[complexity][offset]
	/// Максимальная ширина подзадачи со сложностью не более `complexity`
	/// и начинающейся с позиции `offset`.
	map<size_t, vector<size_t>> widest_from;

	// Аналогично widest_from, но для подзадач, окончанивающихся в `offset`.
	map<size_t, vector<size_t>> widest_to;

	widest_from.emplace(0, vector<size_t>(n, 0));
	widest_to.emplace(0, vector<size_t>(n, 0));

	for (size_t complexity = 1; ; ++complexity) {
		widest_from.emplace(complexity, vector<size_t>(n, 0));
		widest_to.emplace(complexity, vector<size_t>(n, 0));

		for (size_t pivot = 0; pivot < n; ++pivot) {
			if (case_data[pivot] > complexity) {
				continue;
			}

			size_t complexity_left = complexity - case_data[pivot];
			size_t left = pivot>0 ? widest_to[complexity_left][pivot-1] : 0;
			size_t right = pivot<n-1 ? widest_from[complexity_left][pivot+1] : 0;
			if (1 + left + right == n) {
				return complexity;
			}

			if (widest_from[complexity][pivot-left] < 1 + left + right)
				widest_from[complexity][pivot-left] = 1 + left + right;
			if (widest_to[complexity][pivot+right] < 1 + left + right)
				widest_to[complexity][pivot+right] = 1 + left + right;
		}

		{
			size_t acc = 0;
			for (size_t i = 0; i < n; ++i) {
				auto& val = widest_from[complexity][i];
				if (val >= acc) {
					acc = val;
				} else {
					val = acc;
				}
				if (acc) --acc;
			}
		}
		{
			size_t acc = 0;
			for (size_t i = 0; i < n; ++i) {
				auto& val = widest_to[complexity][n-i-1];
				if (val >= acc) {
					acc = val;
				} else {
					val = acc;
				}
				if (acc) --acc;
			}
		}

		if (complexity>=10) {
			widest_from.erase(complexity - 10);
			widest_to.erase(complexity - 10);
		}
	}

	return 0;
}

int main() {
	size_t n;
	cin >> n;

	for (size_t case_num = 1; case_num <= n; ++case_num) {
		string case_str;
		cin >> case_str;

		vector<size_t> case_data;
		transform(
			case_str.begin(), case_str.end(),
			back_inserter(case_data),
			[](const char& c) { return c - '0'; }
		);

		size_t answer = solve(case_data);
		cout << "Case #" << case_num << ": " << answer << endl;
	}

	return 0;
}
