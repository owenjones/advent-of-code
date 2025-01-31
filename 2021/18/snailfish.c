#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>

#include "snailfish.h"

#ifdef DEBUG
#include <stdarg.h>
#endif

void debug(const char *format, ...)
{
#ifdef DEBUG
  va_list arglist;
  va_start(arglist, format);
  vprintf(format, arglist);
  va_end(arglist);
#endif
  return;
}

node_t *init_node(node_t *parent, uint8_t depth)
{
  node_t *node = malloc(sizeof(node_t));
  node->value = -1;
  node->parent = parent;
  node->depth = depth;
  node->left = NULL;
  node->right = NULL;
  node->lr = 0;
  return node;
}

void free_tree(node_t *root)
{
  if (root->left != NULL)
    free_tree(root->left);

  if (root->right != NULL)
    free_tree(root->right);

  free(root);
}

node_t *parse_line(char *line)
{
  node_t *root = init_node(NULL, 1);
  node_t *current = root;

  size_t len = strlen(line);
  for (size_t i = 1; i < len - 1; i++)
  {
    if (current->left == NULL)
      current->left = init_node(current, (current->depth + 1));

    if (current->right == NULL)
      current->right = init_node(current, (current->depth + 1));

    if (line[i] == '[')
    {
      current = (current->lr == 0) ? current->left : current->right;
    }
    else if (line[i] == ',')
    {
      current->lr = 1;
    }
    else if (line[i] == ']')
    {
      current = current->parent;
    }
    else if (line[i] == '0' || isdigit(line[i]) > 0)
    {
      char d = atoi(&line[i]);
      if (current->lr == 0)
      {
        current->left->value = d;
      }
      else
      {
        current->right->value = d;
      }
    }
  }

  return root;
}

uint8_t explode(node_t *node)
{
  if (node != NULL)
  {
    if (node->depth > 4 && node->left != NULL && node->right != NULL && node->left->value > -1 && node->right->value > -1)
    {
      node_t *n;

      // LHS
      n = node;
      while (n->depth > 1)
      {
        if (n->parent->left != n)
        {
          n = n->parent->left;
          break;
        }

        n = n->parent;
      }

      if (n->parent != NULL)
      {
        while (n->value < 0)
          n = n->right;

        n->value += node->left->value;
      }

      // RHS
      n = node;
      while (n->depth > 1)
      {
        if (n->parent->right != n)
        {
          n = n->parent->right;
          break;
        }

        n = n->parent;
      }

      if (n->parent != NULL)
      {
        while (n->value < 0)
          n = n->left;

        n->value += node->right->value;
      }

      node->value = 0;
      free(node->left);
      free(node->right);
      node->left = NULL;
      node->right = NULL;
      return 1;
    }
    else
    {
      uint8_t l = explode(node->left);
      uint8_t r = explode(node->right);
      return l | r;
    }
  }

  return 0;
}

uint8_t split(node_t *node)
{
  if (node->value > -1)
  {
    if (node->value >= 10)
    {
      float v = (float)node->value / 2.0;

      node->left = init_node(node, (node->depth + 1));
      node->left->value = (int)floor(v);

      node->right = init_node(node, (node->depth + 1));
      node->right->value = (int)ceil(v);

      node->value = -1;
      return 1;
    }

    return 0;
  }

  uint8_t l = split(node->left);
  if (l == 1)
    return 1;

  uint8_t r = split(node->right);
  if (r == 1)
    return 1;

  return 0;
}

void reduce(node_t *root)
{
  while (1)
  {
    explode(root);
    if (split(root))
      continue;

    break;
  }
}

void increase_depth(node_t *node)
{
  node->depth += 1;

  if (node->left != NULL)
    increase_depth(node->left);

  if (node->right != NULL)
    increase_depth(node->right);
}

node_t *add_numbers(node_t *left, node_t *right)
{
  node_t *root = init_node(NULL, 1);
  root->left = left;
  root->right = right;
  increase_depth(root->left);
  increase_depth(root->right);
  root->left->parent = root;
  root->right->parent = root;
  return root;
}

uint32_t calculate_magnitude(node_t *node)
{
  if (node->value > -1)
    return node->value;

  return (3 * calculate_magnitude(node->left)) + (2 * calculate_magnitude(node->right));
}

void _to_string(node_t *node)
{
  if (node->value > -1)
  {
    debug("%i", node->value);
  }
  else
  {
    debug("[");
    _to_string(node->left);
    debug(",");
    _to_string(node->right);
    debug("]");
  }
}

void to_string(node_t *node)
{
  _to_string(node);
  debug("\n");
}

// void test_parse_and_magnitude()
// {
//   node_t root = {};

//   char lines[][256] = {
//       "[[1,2],[[3,4],5]]",
//       "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]",
//       "[[[[5,0],[7,4]],[5,5]],[6,6]]",
//       "[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]",
//   };
//   int magnitudes[] = {143, 1384, 1137, 3488};

//   for (size_t i = 0; i < 4; i++)
//   {
//     init_node(&root);
//     parse_line(&root, lines[i]);
//     uint32_t m = calculate_magnitude(&root);
//     debug("%s => %i\n", lines[i], m);
//     assert(m == magnitudes[i]);
//   }
// }

// void test_reduce()
// {
//   node_t root = {};
//   init_node(&root);
//   // parse_line(&root, "[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]");
//   // parse_line(&root, "[[[[[9,8],1],2],3],4]");
//   // parse_line(&root, "[[6,[5,[4,[3,2]]]],1]");
//   to_string(&root);
//   // explode(&root);
//   // split(&root);
//   reduce(&root);
//   to_string(&root);
// }