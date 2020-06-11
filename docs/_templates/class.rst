{{ fullname | replace("skia.", "") | escape | underline }}

{% set classes = members | reject("in", methods) | reject("in", attributes) | reject("ge", "_") | list %}

.. currentmodule:: {{ module }}

.. autoclass:: {{ fullname }}

   {% block classes %}
   {% if classes %}
   .. rubric:: Classes

   .. autosummary::
      :toctree:
      :nosignatures:
      :template: class.rst
   {% for item in classes %}
      ~{{ fullname }}.{{ item }}
   {%- endfor %}
   {%- endif %}
   {% endblock %}

   {% block methods %}
   {% if methods %}
   .. rubric:: Methods

   .. autosummary::
      :nosignatures:
   {% for item in methods %}
      ~{{ fullname }}.{{ item }}
   {%- endfor %}
   {% endif %}
   {% endblock %}

   {% block attributes %}
   {% if attributes %}
   .. rubric:: Attributes

   .. autosummary::
   {% for item in attributes %}
      ~{{ fullname }}.{{ item }}
   {%- endfor %}
   {% endif %}
   {% endblock %}

{% block methods_detail %}
{% if methods %}

Methods
-------

{% for item in methods %}

.. automethod:: {{ module }}.{{ objname }}.{{ item }}

{%- endfor %}
{% endif %}
{% endblock %}

{% block attributes_detail %}
{% if attributes %}

Attributes
----------

{% for item in attributes %}

.. autoattribute:: {{ module }}.{{ objname }}.{{ item }}

{%- endfor %}
{% endif %}
{% endblock %}
