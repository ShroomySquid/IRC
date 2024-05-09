/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbarrett <fbarrett@student.42quebec>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 19:16:24 by fbarrett          #+#    #+#             */
/*   Updated: 2024/05/09 19:27:55 by fbarrett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/IRC.hpp"

int main(int argc, char** argv) {
	if (argc != 3)
	{
		cerr << "Invalid amount of arguments provided. Please provide a port";
		cerr << " and a server password." << endl;
		return (1);
	}
	(void)argv;
	cout << "Hello World!" << endl;
	return (0);
}
